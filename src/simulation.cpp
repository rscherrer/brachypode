// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "simulation.h"

// Lambda for removing dead individuals
auto burry = [](Individual ind) -> bool
{
    return !ind.isAlive();
};

// Main simulation function
int simulate(const std::vector<std::string> &args) {

    try
    {

        // Check arguments
        if (args.size() > 2u)
            throw std::runtime_error("Too many arguments");

        // Create a default parameter set
        Parameters pars;

        // Read parameters from a file if supplied
        if (args.size() > 1u) pars.read(args[1u]);

        // Save parameters if necessary
        if (pars.savepars) pars.save();

        // Initialize a population of individuals
        std::vector<Individual> pop(pars.popsize, {pars.allfreq, pars.nloci, pars.effect});

        // Number of demes
        const size_t ndemes = pars.pgood.size();

        // Create a genetic architecture
        Architecture arch(pars.nchrom, pars.nloci);

        // If necessary...
        if (pars.loadarch) {

            // Load the genetic architecture from a file
            arch.load();

            // Update hyperparameters accordingly
            pars.nchrom = arch.chromends.size();
            pars.nloci = arch.locations.size();

        }

        // Save the architecture if necessary
        if (pars.savearch) arch.save(pars);

        // Save parameters if necessary
        if (pars.savepars) pars.save();

        // Redirect output to log file if needed
        if (pars.savelog) pars.savelog = std::freopen("log.txt", "w", stdout);

        // Create a vector of output file streams (using smart pointers)
        std::vector<std::shared_ptr<std::ofstream> > outfiles;

        // Which data files to save
        std::vector<std::string> filenames = {"time", "popsize", "patchsizes", "traitmeans", "individuals"};

        // Update the files to save if needed...
        if (pars.choose) {

            // Read file where those are provided
            std::ifstream infile("whattosave.txt");
            if (!infile.is_open())
                throw std::runtime_error("Could not read input file whattosave.txt");
            std::vector<std::string> newfilenames;
            std::string input;
            while (infile >> input) newfilenames.push_back(input);
            stf::check(newfilenames, filenames);
            filenames.reserve(newfilenames.size());
            filenames.resize(newfilenames.size());
            for (size_t f = 0u; f < newfilenames.size(); ++f)
                filenames[f] = newfilenames[f];

        }

        // Open the file streams
        stf::open(outfiles, filenames);

        // Set up flags for which data to save
        int timeFile(-1), popsizeFile(-1), patchsizesFile(-1), traitmeansFile(-1), individualsFile(-1);
        for (size_t f = 0u; f < filenames.size(); ++f) {

            const std::string filename = filenames[f];

            if (filename == "time") timeFile = f;
            else if (filename == "popsize") popsizeFile = f;
            else if (filename == "patchsizes") patchsizesFile = f;
            else if (filename == "traitmeans") traitmeansFile = f;
            else if (filename == "individuals") individualsFile = f;
            else throw std::runtime_error("Invalid output requested in whattosave.txt");

        }

        std::cout << "Simulation started.\n";

        // Sow the individuals at random if needed
        if (pars.sow) {

            auto pickDeme = rnd::random(0u, ndemes - 1u);

            for (size_t i = 0u; i < pop.size(); ++i) {

                const size_t newdeme = pickDeme(rnd::rng);
                auto pickPatch = rnd::bernoulli(pars.pgood[newdeme]);
                const size_t newpatch = pickPatch(rnd::rng);
                pop[i].setDeme(newdeme);
                pop[i].setPatch(newpatch);

            }

        }

        // At each time step...
        for (size_t t = 0u; t <= pars.tend; ++t) {

            // Flag to know if it is time to save some data
            const bool timetosave = t % pars.tsave == 0u;

            // Compute useful population statistics
            size_t popsize = 0u;
            std::vector<size_t> demesizes(ndemes, 0u);
            std::vector<size_t> patchsizes(2u * ndemes, 0u);

            // Containers for mean traits in each deme and each patch
            const size_t npatches = 2u * ndemes;
            std::vector<double> meanx(npatches, 0.0);

            // Loop through individuals to calculate them
            for (size_t i = 0u; i < pop.size(); ++i) {

                // Get trait values and locations
                const double x = pop[i].getX();
                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();

                // Save individual-level data if needed
                if (timetosave && individualsFile >= 0) {

                    const double deme_ = static_cast<double>(pop[i].getDeme());
                    const double patch_ = static_cast<double>(pop[i].getPatch());
                    outfiles[individualsFile]->write((char *) &deme_, sizeof(double));
                    outfiles[individualsFile]->write((char *) &patch_, sizeof(double));
                    outfiles[individualsFile]->write((char *) &x, sizeof(double));

                }

                // Update statistics
                ++popsize;
                ++demesizes[deme];
                const size_t j = 2u * deme + patch;
                ++patchsizes[j];
                meanx[j] += x;

            }

            // Save time if needed
            if (timetosave && timeFile >= 0) {
                const double t_ = static_cast<double>(t);
                outfiles[timeFile]->write((char *) &(t_), sizeof(double));
            }

            // Save population size if needed
            if (timetosave && popsizeFile >= 0) {
                const double popsize_ = static_cast<double>(popsize);
                outfiles[popsizeFile]->write((char *) &popsize_, sizeof(double));
            }

            // Save patch sizes if needed
            if (timetosave && patchsizesFile >= 0) {

                for (size_t j = 0u; j < patchsizes.size(); ++j) {

                    const double patchsize_ = static_cast<double>(patchsizes[j]);
                    outfiles[patchsizesFile]->write((char *) &patchsize_, sizeof(double));

                }

            }

            // Convert sums into a mean...
            for (size_t j = 0u; j < patchsizes.size(); ++j) {

                const size_t n = patchsizes[j];

                if (n > 0u) meanx[j] /= n;

                // Save trait means if needed
                if (timetosave && traitmeansFile >= 0)
                    outfiles[traitmeansFile]->write((char *) &meanx[j], sizeof(double));

            }

            // Verbose if needed
            if (pars.talkative) {

                std::cout << "n = { ";
                for (size_t j = 0u; j < demesizes.size(); ++j) std::cout << demesizes[j] << ' ';
                std::cout << "} at t = " << t << '\n';

            }

            // Prepare to count the total number of offspring
            size_t nseeds = 0u;

            // Number of adult plants
            const size_t nadults = pop.size();

            // For each adult plant...
            for (size_t i = 0u; i < nadults; ++i) {

                // Extract relevant information
                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();
                const double x = pop[i].getX();
                
                // Parameters that apply to the current patch
                const double stress = pars.stress[patch];
                const double capacity = pars.capacities[patch];

                // Current local population size
                const size_t n = patchsizes[2u * deme + patch];

                // Compute fitness
                double fitness = pars.maxgrowth - pars.tradeoff * x;
                fitness /= (1.0 + exp(pars.steep * (stress - x)));
                fitness -= n / capacity;

                // Fitness should be positive
                assert(fitness > 0.0);

                // Sample the number of surviving offspring
                size_t noff = 0u;
                if (fitness > 0.0) noff = rnd::poisson(fitness)(rnd::rng);
                nseeds += noff;

                // For each surviving offspring...
                for (size_t j = 0u; j < noff; ++j) {

                    // Add the clone to the population
                    pop.push_back(pop[i]);

                    // If it is the product of outcrossing...
                    if (rnd::bernoulli(1.0 - pars.selfing)(rnd::rng)) {

                        // Select another adult at random to provide pollen
                        size_t k = rnd::random(1u, nadults - 1u)(rnd::rng);
                        if (k < i) k -= 1u;

                        // Recombine the genomes of the two parents
                        pop.back().recombine(pars.recombination, pop[k], arch.chromends, arch.locations);

                    }

                    // If the seed disperses to another site...
                    if (ndemes > 1u && rnd::bernoulli(pars.longrange)(rnd::rng)) {

                        // Sample destination deme
                        size_t newdeme = rnd::random(1u, ndemes - 1u)(rnd::rng);
                        if (newdeme < deme) newdeme -= 1u;
                        pop.back().setDeme(newdeme);

                        // Lands in a random patch
                        auto pickPatch = rnd::bernoulli(pars.pgood[pop.back().getDeme()]);
                        pop.back().setPatch(pickPatch(rnd::rng));

                    } else {

                        // Otherwise, the seed may still disperse within the local site depending on the cover of the other patch
                        const double cover = pars.pgood[pop.back().getDeme()];
                        const double target = pop.back().getPatch() ? 1.0 - cover : cover; 

                        // If that happens...
                        if (rnd::bernoulli(pars.shortrange * target)(rnd::rng)) {

                            // Change patch
                            const size_t newpatch = (pop.back().getPatch() - 1u) % 2u;
                            pop.back().setPatch(newpatch);

                        }

                    }

                    // Does it mutate?
                    pop.back().mutate(pars.mutation, pars.nloci);

                    // Update trait value based on its genes
                    pop.back().develop(pars.effect);

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

                std::cout << "The population went extinct at t = " << t << '\n';
                break;

            }
        }

        std::cout << "Simulation ended.\n";
        if (pars.savelog) std::fclose(stdout);

        // Close output file streams
        stf::close(outfiles);

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