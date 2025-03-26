// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "MAIN.hpp"
#include "buffer.hpp"
#include "population.hpp"

// Function to return a list of valid outputs
std::vector<std::string> valid() {

    // Return a list of valid outputs
    return {"time", "popsize", "patchsizes", "traitmeans", "individuals"};

}

// Main simulation function
void doMain(const std::vector<std::string> &args) {

    // args: program arguments

    // Error if two many arguments
    if (args.size() > 2u) 
        throw std::runtime_error("Too many arguments provided");

    // Parameter file
    const std::string parfile = args.size() == 2u ? args[1u] : "";

    // Create parameters (from file if needed)
    const Parameters pars(parfile);

    // Redirect output to log file if needed
    if (pars.savelog) std::freopen("log.txt", "w", stdout);

    // Verbose (after redirecting output)
    if (args.size() == 2u) std::cout << "Parameters read in succesfully\n";

    // Seed the random number generator
    rnd::rng.seed(pars.seed);

    // Architecture file
    const std::string archfile = pars.loadarch ? "architecture.txt" : "";

    // Genetic architecture (from file if needed)
    const Architecture arch({pars.nchrom, pars.nloci, pars.effect}, archfile);
    
    // Note: genetic parameters from the architecture now override
    // those that may have been given in the general parameters.

    // Verbose
    if (pars.loadarch) std::cout << "Genetic architecture read in succesfully\n";

    // Save the architecture if necessary
    if (pars.savearch) arch.save("architecture.txt");

    // Save parameters if necessary
    if (pars.savepars) pars.save("paramlog.txt");
    
    // TODO: Check the right headers are included in the right files

    // Create a printer
    Printer print(valid());

    // If needed...
    if (pars.savedat && pars.choose) {
        
        // Read outputs to save
        print.read("whattosave.txt");

        // Verbose
        std::cout << "Requested output(s) read in succesfully\n";

    }

    // Open the buffers if needed
    if (pars.savedat) print.open();

    // Create a population of individuals
    Population pop(pars, arch);
    
    // Verbose
    std::cout << "Simulation started\n";

    // At each time step...
    while (pop.keepon()) {

        // Print progress if needed
        if (pars.verbose) pop.show();

        // Update climate parameters if needed
        pop.update();

        // Go through the life cycle of the population
        pop.cycle(print);

        // End if extinction
        if (pop.extinct()) break;

        // Move on
        pop.moveon();

    }

    // End of simulation
    std::cout << "Simulation ended\n";

    // Close the log file if needed
    if (pars.savelog) std::fclose(stdout);
    
    // Close the streams to output files
    if (pars.savedat) print.close();

}