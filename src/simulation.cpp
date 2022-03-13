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
        if (args.size() > 2u)
            throw std::runtime_error("More than one argument were supplied");

        // Create a default parameter set
        Parameters pars;

        // Read parameters from a file if supplied
        if (args.size() == 2u) pars.read(args[1u]);

        // Save parameters if necessary
        if (pars.savepars) pars.save();

        // Initialize a population of individuals
        std::vector<Individual> pop(pars.popsize);

        // Number of demes
        const size_t ndemes = pars.pgood.size();

        // Create a genetic architecture
        Architecture arch(pars.nchrom, pars.nloci);

        // Load the genetic architecture if necessary
        if (pars.loadarch) {
            arch.load();
            pars.nchrom = arch.chromends.size();
            pars.nloci = arch.locations.size();
        }

        // Save the architecture if necessary
        if (pars.savearch) arch.save(pars);

        // Save parameters if necessary
        if (pars.savepars) pars.save();

        // Redirect output to log file if needed
        if (pars.savelog) pars.savelog = std::freopen("log.txt", "w", stdout);

        std::cout << "Simulation started.\n";

        // At each time step...
        for (size_t t = 0u; t < pars.tend; ++t) {

            // Track deme and patch sizes
            std::vector<size_t> demesizes(ndemes, 0u);
            std::vector<std::vector<size_t>> patchsizes(ndemes, {0u, 0u});
            for (size_t i = 0u; i < pop.size(); ++i) {

                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();
                ++demesizes[deme];
                ++patchsizes[deme][patch];

            }

            std::cout << "n = { ";
            for (size_t j = 0u; j < ndemes; ++j) std::cout << demesizes[j] << ' ';
            std::cout << "} at t = " << t << '\n';

            // Prepare to count the total number of offspring
            size_t nseeds = 0u;

            // Number of adult plants
            const size_t nadults = pop.size();

            // For each adult plant...
            for (size_t i = 0u; i < nadults; ++i) {

                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();
                const double x = pop[i].getX();
                const double maxgrowth = pars.maxgrowths[patch];
                const double xopt = pars.xopts[patch];
                const double xwidth = pars.xwidths[patch];
                const double capacity = pars.capacities[patch];

                // Compute fitness depending trait and patch
                double fitness = maxgrowth * exp(-utl::sqr((x - xopt) / xwidth));

                // Account for local competition within deme and within patch
                fitness *= (1.0 - patchsizes[deme][patch] / capacity);

                // Sample the number of surviving offspring
                const size_t noff = rnd::poisson(fitness)(rnd::rng);
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

                    // If the seed disperses...
                    if (rnd::bernoulli(pars.dispersal)(rnd::rng)) {

                        // Sample destination deme
                        size_t newdeme = rnd::random(1u, ndemes - 1u)(rnd::rng);
                        if (newdeme < deme) newdeme -= 1u;
                        pop.back().setDeme(newdeme);

                    }

                    // Which patch does it land in?
                    pop.back().setPatch(rnd::bernoulli(pars.pgood[pop.back().getDeme()])(rnd::rng));

                    // Does it mutate?
                    pop.back().mutate(pars.mutation, pars.nloci);

                    // Update trait value based on its genes
                    pop.back().develop(pars.effect);

                }
            }

            // All adults die
            for (size_t i = 0u; i < pop.size() - nseeds; ++i) pop[i].kill();
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

