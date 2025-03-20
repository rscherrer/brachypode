// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "simulation.hpp"
#include "buffer.hpp"

// Function to return a list of valid outputs
std::vector<std::string> valid() {

    // Return a list of valid outputs
    return {"time", "popsize", "patchsizes", "traitmeans", "individuals"};

}

// Lambda for removing dead individuals
auto burry = [](Individual ind) -> bool
{
    return !ind.isAlive();
};

// Main simulation function
int simulate(const std::vector<std::string> &args) {

    try
    {

        // Create a default parameter set
        Parameters pars;

        // Error if two many arguments
        if (args.size() > 2u) 
            throw std::runtime_error("Too many arguments provided.");

        // Read parameters from a file if supplied
        if (args.size() == 2u) pars.read(args[1u]);

        // Seed the random number generator
        rnd::rng.seed(pars.seed);

        // Create a genetic architecture
        Architecture arch(pars.nchrom, pars.nloci, pars.effect);

        // If necessary...
        if (pars.loadarch) {

            // Load the genetic architecture from a file
            arch.load();

            // Update hyperparameters accordingly
            pars.nchrom = arch.chromends.size();
            pars.nloci = arch.locations.size();

        }

        // Save the architecture if necessary
        if (pars.savearch) arch.save();

        // Save parameters if necessary
        if (pars.savepars) pars.save();

        // Prepare a population of individuals
        std::vector<Individual> pop;

        // Reserve space for the population
        pop.reserve(pars.popsize);

        // Fill the population with individuals
        for (size_t i = 0u; i < pars.popsize; ++i) 
            pop.emplace_back(Individual(pars.allfreq, arch.effects));

        // TODO: Would it make more sense to have the architecture directly available to the individuals?
        // TODO: Should I pass Architecture objects or their members to the individuals?
        // TODO: Should the main simulation function be its own source file separate from main?
        // TODO: Check consistency of when we return messages to the screen
        // TODO: Check the right headers are included in the right files

        // Number of demes
        const size_t ndemes = pars.pgood.size();

        // Useful exponent that might be needed for non-linear trade-offs
        const double d = -log2(1.0 - pars.tradeoff);

        // Maximum trait value
        double xmax = 0.0;
        for (size_t l = 0u; l < arch.effects.size(); ++l) xmax += arch.effects[l];

        // Redirect output to log file if needed
        if (pars.savelog) std::freopen("log.txt", "w", stdout);

        // Set the list of outputs to save
        std::vector<std::string> outputs = valid();

        // Overwrite with user choices if needed
        if (pars.savedat && pars.choose) stf::read(outputs, "whattosave.txt", valid());

        // Create an unordered map of (optional) buffers
        std::unordered_map<std::string, std::optional<Buffer> > buffers;

        // Open the buffers if needed
        if (pars.savedat) stf::open(buffers, outputs);

        // Verbose
        std::cout << "Simulation started.\n";

        // If needed...
        if (pars.sow) {

            // Prepare a random deme sampler
            auto pickDeme = rnd::random(0u, ndemes - 1u);

            // For each individual...
            for (size_t i = 0u; i < pop.size(); ++i) {

                // Sample a deme 
                const size_t newdeme = pickDeme(rnd::rng);

                // Prepare a patch sampler based on good patch cover
                auto pickPatch = rnd::bernoulli(pars.pgood[newdeme]);

                // Sample a patch
                const size_t newpatch = pickPatch(rnd::rng);

                // Update the individual
                pop[i].setDeme(newdeme);
                pop[i].setPatch(newpatch);

            }
        }

        // At each time step...
        for (size_t t = 0u; t <= pars.tend; ++t) {

            // Climate change if needed
            pars.update(t);

            // Flag to know if it is time to save some data
            const bool timetosave = t % pars.tsave == 0u;

            // Compute useful population statistics
            size_t popsize = 0u;
            std::vector<size_t> demesizes(ndemes, 0u);
            std::vector<size_t> patchsizes(2u * ndemes, 0u);

            // Containers for mean traits in each deme and each patch
            const size_t npatches = 2u * ndemes;
            std::vector<double> meanx(npatches, 0.0);

            // Loop through individuals to calculate means
            for (size_t i = 0u; i < pop.size(); ++i) {

                // Get trait values and locations
                const double x = pop[i].getX();
                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();

                // If needed...
                if (timetosave && buffers["individuals"]) {

                    // Convert to doubles
                    const double deme_ = static_cast<double>(pop[i].getDeme());
                    const double patch_ = static_cast<double>(pop[i].getPatch());

                    // Store individual-level data
                    buffers["individuals"]->store(deme_);
                    buffers["individuals"]->store(patch_);
                    buffers["individuals"]->store(x);

                }

                // Update statistics
                ++popsize;
                ++demesizes[deme];
                const size_t j = 2u * deme + patch;
                ++patchsizes[j];
                meanx[j] += x;

            }

            // If needed...
            if (timetosave && buffers["time"]) {

                // Convert to double
                const double t_ = static_cast<double>(t);

                // Save time point
                buffers["time"]->store(t_);
            }

            // If needed...
            if (timetosave && buffers["popsize"]) {

                // Convert to double
                const double popsize_ = static_cast<double>(popsize);

                // Save population size
                buffers["popsize"]->store(popsize_);
            }

            // If needed...
            if (timetosave && buffers["patchsizes"]) {

                // For each patch...
                for (size_t j = 0u; j < patchsizes.size(); ++j) {

                    // Convert to double
                    const double patchsize_ = static_cast<double>(patchsizes[j]);

                    // Save patch size
                    buffers["patchsizes"]->store(patchsize_);

                }

            }

            // For each deme...
            for (size_t j = 0u; j < patchsizes.size(); ++j) {

                // Get patch size
                const size_t n = patchsizes[j];

                // Use it to compute the mean trait value
                if (n > 0u) meanx[j] /= n;

                // Save trait means if needed
                if (timetosave && buffers["traitmeans"]) buffers["traitmeans"]->store(meanx[j]);

            }

            // If needed...
            if (pars.verbose) {

                // Verbose
                std::cout << "n = { ";
                for (size_t j = 0u; j < demesizes.size(); ++j) std::cout << demesizes[j] << ' ';
                std::cout << "} at t = " << t << '\n';

            }

            // Number of adult plants
            const size_t nadults = pop.size();

            // Prepare a partner sampler for outcrossing
            auto getPollen = rnd::random(1u, nadults - 1u);

            // Prepare a deme sampler for seed dispersal
            auto getDestination = rnd::random(1u, ndemes - 1u);

            // For each adult plant...
            for (size_t i = 0u; i < nadults; ++i) {

                // Extract relevant information
                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();
                const double x = pop[i].getX();
                
                // Current local population size
                const size_t n = patchsizes[2u * deme + patch];

                // Population growth rate
                double r = pars.maxgrowth - pars.tradeoff * x;

                // Modify if needed
                if (pars.type == 2u) r = pars.maxgrowth * pow(1.0 - pow(x / xmax, 1.0 / d), d);

                // Shrub cover in the current deme
                const double pgood = pars.pgood[deme];

                // Cover of the focal patch in the deme
                const double cover = patch ? pgood : 1.0 - pgood;

                // Total carrying capacity in the deme for the focal patch
                const double Ktot = pars.capacities[patch] * cover;

                // Expected number of seeds
                const double enseeds = exp((r > 0.0 ? r : 0.0) * (1.0 - n / Ktot));

                // Realized number of seeds
                const size_t nseeds = rnd::poisson(enseeds)(rnd::rng);

                // For each seed produced...
                for (size_t j = 0u; j < nseeds; ++j) {

                    // Add a clone of the parent to the population
                    pop.push_back(pop[i]);

                    // If the seed is the product of outcrossing...
                    if (rnd::bernoulli(1.0 - pars.selfing)(rnd::rng)) {

                        // Select another adult at random to provide pollen
                        size_t k = getPollen(rnd::rng);

                        // Avoid selfing
                        if (k < i) k -= 1u;

                        // Recombine the genomes of the two parents
                        pop.back().recombine(pars.recombination, pop[k], arch.chromends, arch.locations);

                    }

                    // If the seed disperses to another site...
                    if (ndemes > 1u && rnd::bernoulli(pars.dispersal)(rnd::rng)) {

                        // Sample destination deme
                        size_t newdeme = getDestination(rnd::rng);

                        // Avoid the current deme
                        if (newdeme < deme) newdeme -= 1u;

                        // Send the seed there
                        pop.back().setDeme(newdeme);

                    }

                    // Prepare to sample landing patch based on good patch cover
                    auto pickPatch = rnd::bernoulli(pars.pgood[pop.back().getDeme()]);

                    // Set the patch of the seed
                    pop.back().setPatch(pickPatch(rnd::rng));

                    // Does the seed mutate?
                    pop.back().mutate(pars.mutation, pars.nloci);

                    // Update trait value based on its genes
                    pop.back().develop(arch.effects);

                    // Stress level in the patch where the seed has landed
                    const double stress = pars.stress[pop.back().getPatch()];

                    // Trait value of the seedling
                    const double xoff = pop.back().getX();

                    // Compute the survival probability of the seedling
                    const double prob = 1.0 / (1.0 + exp(pars.steep * (stress - xoff)));

                    // Remove the seedling if it does not survive
                    if (!rnd::bernoulli(prob)(rnd::rng)) pop.pop_back();

                }

                // Kill the current adult (will be removed after reproduction)
                pop[i].kill();

            }

            // Remove dead plants
            auto it = std::remove_if(pop.begin(), pop.end(), burry);
            pop.erase(it, pop.end());
            pop.shrink_to_fit();

            // Is the population still there?
            if (pop.size() == 0u) {

                // If not say it and stop
                std::cout << "The population went extinct at t = " << t << '\n';
                break;

            }
        }

        // End of simulation
        std::cout << "Simulation ended.\n";

        // Close the log file if needed
        if (pars.savelog) std::fclose(stdout);
        
        // Close the streams to output files
        if (pars.savedat) stf::close(buffers);

        return 0;

    }
    catch (const std::exception& err)
    {
        std::cerr << "Exception: " << err.what() << '\n';
    }
    catch (const char* err)
    {
        std::cerr << "Exception: " << err << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown Exception\n";
    }
    return 1;

}