#include "parameters.h"

Parameters::Parameters() :
    popsize(100u),
    pgood({0.8, 0.6, 0.5, 0.3, 0.1}),
    maxgrowths({1.0, 2.0}),
    xopts({3.0, 1.0}),
    xwidths({4.0, 1.0}),
    capacities({10000.0, 100.0}),
    dispersal(0.1),
    mutation(0.001),
    nchrom(1u),
    nloci(20u),
    effect(0.3),
    selfing(0.95),
    recombination(3.0),
    tend(100u),
    seed(makeDefaultSeed()),
    loadarch(false),
    savepars(true),
    savelog(false),
    savearch(true)
{

    // Make sure parameter values make sense
    check();

    // Seed the random number generator
    rnd::rng.seed(seed);
}

// Create a default seed based on clock
size_t Parameters::makeDefaultSeed()
{
    return static_cast<size_t>(std::chrono::high_resolution_clock::now().
     time_since_epoch().count());
}

// Read parameters from a file
void Parameters::read(const std::string &filename)
{
    std::ifstream inputfile;
    inputfile.open(filename);
    if (!inputfile.is_open()) {
        std::string msg = "Unable to open parameter file ";
        throw std::runtime_error(msg + filename);
    }

    import(inputfile);
    inputfile.close();
}

// Import parameters from a file stream
void Parameters::import(std::ifstream &file)
{

    std::string input;
    size_t ndemes;

    while (file >> input) {

        if (input == "popsize") file >> popsize;
        else if (input == "pgood") {
            file >> ndemes;
            if (!ndemes) throw std::runtime_error("Zero deme provided");
            pgood = std::vector<double>(ndemes);
            for (size_t i = 0u; i < ndemes; ++i)
                file >> pgood[i];
        }
        else if (input == "maxgrowths") for (size_t i = 0u; i < 2u; ++i) file >> maxgrowths[i];
        else if (input == "xopts") for (size_t i = 0u; i < 2u; ++i) file >> xopts[i];
        else if (input == "xwidths") for (size_t i = 0u; i < 2u; ++i) file >> xwidths[i];
        else if (input == "capacities") for (size_t i = 0u; i < 2u; ++i) file >> capacities[i];
        else if (input == "dispersal") file >> dispersal;
        else if (input == "mutation") file >> mutation;
        else if (input == "nchrom") file >> nchrom;
        else if (input == "nloci") file >> nloci;
        else if (input == "effect") file >> effect;
        else if (input == "selfing") file >> selfing;
        else if (input == "recombination") file >> recombination;
        else if (input == "tend") file >> tend;
        else if (input == "seed") file >> seed;
        else if (input == "loadarch") file >> loadarch;
        else if (input == "savepars") file >> savepars;
        else if (input == "savelog") file >> savelog;
        else if (input == "savearch") file >> savearch;
        else
            throw std::runtime_error("Invalid parameter name: " + input);

    }

    // Now update interactive parameters
    update();

    std::clog << "Parameters were read in succesfully.\n";

}

// Update the system after having read new parameters
void Parameters::update()
{
    rnd::rng.seed(seed);
    check();
}

// Check that the parameter values are valid
void Parameters::check() const
{
    std::string msg = "No error detected";

    if (popsize == 0u) throw std::runtime_error("Initial population size cannot be zero");
    if (pgood.size() < 1u) throw std::runtime_error("There should be at least one patch");
    for (size_t i = 0u; i < pgood.size(); ++i)
        if (pgood[i] < 0.0 || pgood[i] > 1.0)
            throw std::runtime_error("Proportion of good patches should be between zero and one");
    for (size_t i = 0u; i < 2u; ++i) {
        if (maxgrowths[i] < 0.0) throw std::runtime_error("Maximum growth rate cannot be negative");
        if (xwidths[i] < 0.0) throw std::runtime_error("Niche width cannot be negative");
        if (capacities[i] < 0.0) throw std::runtime_error("Carrying capacity cannot be negative");
    }
    if (dispersal < 0.0 || dispersal > 1.0) throw std::runtime_error("Dispersal rate should be between zero and one");
    if (mutation < 0.0 || mutation > 1.0) throw std::runtime_error("Mutation rate should be between zero and one");
    if (nloci > 1000) throw std::runtime_error("There cannot be more than 1000 loci");
    if (nchrom == 0) throw std::runtime_error("There cannot be zero chromosomes");
    if (nloci == 0u) throw std::runtime_error("There cannot be zero loci");
    if (selfing < 0.0 || selfing > 1.0) throw std::runtime_error("Rate of selfing must be between zero and one");
    if (recombination < 0.0) throw std::runtime_error("Recombination rate cannot be negative");
    if (tend == 0u) throw std::runtime_error("Simulation time cannot be zero");

}

// Save parameters to a file
void Parameters::save() const
{
    const std::string filename = "paramlog.txt";
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);
    write(file);
    file.close();
}

// Write parameters to a file stream
void Parameters::write(std::ofstream &file) const
{

    file << "popsize " << popsize << '\n';
    file << "pgood";
    for (size_t i = 0u; i < pgood.size(); ++i) file << ' ' << pgood[i];
    file << '\n';
    file << "maxgrowths " << maxgrowths[0u] << ' ' << maxgrowths[1u] << '\n';
    file << "xopts " << xopts[0u] << ' ' << xopts[1u] << '\n';
    file << "xwidths " << xwidths[0u] << ' ' << xwidths[1u] << '\n';
    file << "capacities " << capacities[0u] << ' ' << capacities[1u] << '\n';
    file << "dispersal " << dispersal;
    file << "mutation " << mutation;
    file << "nchrom " << nchrom;
    file << "nloci " << nloci;
    file << "effect " << effect;
    file << "selfing" << selfing;
    file << "recombination " << recombination;
    file << "tend " << tend;
    file << "seed " << seed;
    file << "loadarch " << loadarch;
    file << "savepars " << savepars;
    file << "savelog " << savelog;
    file << "savearch " << savearch;

}
