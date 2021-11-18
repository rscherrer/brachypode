#include "simul.h"

// Main simulation function
int simulate(const std::vector<std::string> &args) {

    try
    {
        if (args.size() > 2u)
            throw std::runtime_error("More than one argument were supplied");

        // Create a default parameter set
        Param pars;

        // Read parameters from a file if supplied
        if (args.size() == 2u) pars.read(args[1u]);

        // Save parameters if necessary
        if (pars.savepars) pars.save();

        // Create a landscape
        Landscape lnd = Landscape(pars);

        // Load the landscape if necessary
        if (pars.loadlandscape) lnd.load();

        // Create a genetic architecture
        Architecture arch = Architecture(pars);

        // Load the genetic architecture if necessary
        // if (pars.loadarch) arch.load(pars);

        // Create a population
        Population pop = Population(pars, lnd, arch);

        // At each time step...
        for (size_t t = 0u; t < 10u; ++t) {

            // Assign fitness to individuals
            pop.assignFitnesses(pars, lnd);

            // Reproduction (+ survival and dispersal)
            pop.lifeCycle(lnd);

            // Is the population still there?
            if (pop.getSize() == 0u) {
                std::cout << "The population went extinct at t = " << t << '\n';
                break;
            }

        }

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
