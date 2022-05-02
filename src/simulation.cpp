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
        std::vector<Individual> pop(pars.popsize, {pars.allfreq, pars.nloci});

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
            std::vector<std::string> newfilenames = { };
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

        // At each time step...
        for (size_t t = 0u; t <= pars.tend; ++t) {

            // Flag to know if it is time to save some data
            const bool timetosave = t % pars.tsave == 0u;

            // Compute useful population statistics
            size_t popsize = 0u;
            std::vector<size_t> demesizes(ndemes, 0u);
            std::vector<std::vector<size_t>> patchsizes(ndemes, {0u, 0u});

            // Containers for mean traits in each deme and each patch
            std::vector<std::vector<double> > meanx(ndemes, std::vector<double>(2u, 0.0));
            std::vector<std::vector<double> > meany(ndemes, std::vector<double>(2u, 0.0));
            std::vector<std::vector<double> > meanz(ndemes, std::vector<double>(2u, 0.0));

            // Loop through individuals to calculate them
            for (size_t i = 0u; i < pop.size(); ++i) {

                // Get trait values and locations
                const double x = pop[i].getX();
                const double y = pop[i].getY();
                const double z = pop[i].getZ();
                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();

                // Save individual-level data if needed
                if (timetosave && individualsFile >= 0) {

                    const double deme_ = static_cast<double>(pop[i].getDeme());
                    const double patch_ = static_cast<double>(pop[i].getPatch());
                    outfiles[individualsFile]->write((char *) &deme_, sizeof(double));
                    outfiles[individualsFile]->write((char *) &patch_, sizeof(double));
                    outfiles[individualsFile]->write((char *) &x, sizeof(double));
                    outfiles[individualsFile]->write((char *) &y, sizeof(double));
                    outfiles[individualsFile]->write((char *) &z, sizeof(double));

                }

                // Update statistics
                ++popsize;
                ++demesizes[deme];
                ++patchsizes[deme][patch];
                meanx[deme][patch] += x;
                meany[deme][patch] += y;
                meanz[deme][patch] += z;

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
                    const double patchsize0_ = static_cast<double>(patchsizes[j][0u]);
                    const double patchsize1_ = static_cast<double>(patchsizes[j][1u]);
                    outfiles[patchsizesFile]->write((char *) &patchsize0_, sizeof(double));
                    outfiles[patchsizesFile]->write((char *) &patchsize1_, sizeof(double));
                }
            }

            // Keep sums of competitiveness (y) for later
            std::vector<std::vector<double>> sumys(meany);

            // Convert sums into a mean...
            for (size_t j = 0u; j < demesizes.size(); ++j) {
                for (size_t k = 0u; k < 2u; ++k) {

                    const size_t n = patchsizes[j][k];

                    if (n > 0u) {
                        meanx[j][k] /= n;
                        meany[j][k] /= n;
                        meanz[j][k] /= n;
                    }

                    // Save trait means if needed
                    if (timetosave && traitmeansFile >= 0) {
                        outfiles[traitmeansFile]->write((char *) &meanx[j][k], sizeof(double));
                        outfiles[traitmeansFile]->write((char *) &meany[j][k], sizeof(double));
                        outfiles[traitmeansFile]->write((char *) &meanz[j][k], sizeof(double));
                    }
                }
            }

            // Verbose if needed
            if (pars.talkative) {

                std::cout << "n = { ";
                for (size_t j = 0u; j < ndemes; ++j) std::cout << demesizes[j] << ' ';
                std::cout << "} at t = " << t << '\n';

            }

            // Prepare to count the total number of offspring
            size_t nseeds = 0u;

            // Number of adult plants
            const size_t nadults = pop.size();

            // For each adult plant...
            for (size_t i = 0u; i < nadults; ++i) {

                const size_t deme = pop[i].getDeme();
                const size_t patch = pop[i].getPatch();
                const double x = pop[i].getX();
                const double z = pop[i].getZ();
                const double maxgrowth = pars.type == 1u ? pars.maxgrowths[patch] : pars.maxgrowths[0u];
                const double stress = pars.stress[patch];
                const double zwidth = pars.zwidths[patch];
                const double cover = patch == 0u ? 1.0 - pars.pgood[deme] : pars.pgood[deme];
                const double capacity = pars.capacities[patch] * cover;

                // Initialize fitness
                double fitness = maxgrowth;

                // Compute realized growth rate
                if (pars.type == 1u) fitness *= exp(-utl::sqr((z - stress) / zwidth));
                else if (pars.type == 2u) fitness /= (1.0 + exp(pars.steep * (stress - x)));
                else throw std::runtime_error("Invalid simulation type");

                // Local competition within deme and patches
                const double n = pars.type == 1u ? patchsizes[deme][patch] : sumys[deme][patch];
                fitness *= (1.0 - n / capacity);

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

                    // If the seed disperses...
                    if (ndemes > 1u && rnd::bernoulli(pars.dispersal)(rnd::rng)) {

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
                    pop.back().develop(pars.nloci, pars.effect, pars.xmax, pars.ymax, pars.tradeoff);

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

