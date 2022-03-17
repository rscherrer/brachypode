// This script contains all the functions of the Parameters structure.

#include "parameters.h"

Parameters::Parameters() :
    type(1u),
    popsize(100u),
    pgood({0.8, 0.6, 0.5, 0.3, 0.1}),
    maxgrowths({1.0, 3.0}),
    zopts({3.0, 1.0}),
    zwidths({4.0, 1.0}),
    capacities({10000.0, 100.0}),
    stress({3.0, 0.0}),
    growth(3.0),
    steep(2.0),
    dispersal(0.1),
    mutation(0.001),
    nchrom(1u),
    nloci(20u),
    effect(0.3),
    xmax(10.0),
    ymax(3.0),
    tradeoff(1.0),
    selfing(0.95),
    recombination(3.0),
    tend(100u),
    tsave(20u),
    seed(makeDefaultSeed()),
    loadarch(false),
    savepars(true),
    savelog(false),
    savearch(true),
    talkative(false),
    choose(false)
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

        if (input == "type") file >> type;
        else if (input == "popsize") file >> popsize;
        else if (input == "pgood") {
            file >> ndemes;
            if (!ndemes) throw std::runtime_error("Zero deme provided");
            pgood = std::vector<double>(ndemes);
            for (size_t i = 0u; i < ndemes; ++i)
                file >> pgood[i];
        }
        else if (input == "maxgrowths") for (size_t i = 0u; i < 2u; ++i) file >> maxgrowths[i];
        else if (input == "zopts") for (size_t i = 0u; i < 2u; ++i) file >> zopts[i];
        else if (input == "zwidths") for (size_t i = 0u; i < 2u; ++i) file >> zwidths[i];
        else if (input == "capacities") for (size_t i = 0u; i < 2u; ++i) file >> capacities[i];
        else if (input == "stress") for (size_t i = 0u; i < 2u; ++i) file >> stress[i];
        else if (input == "growth") file >> growth;
        else if (input == "steep") file >> steep;
        else if (input == "dispersal") file >> dispersal;
        else if (input == "mutation") file >> mutation;
        else if (input == "nchrom") file >> nchrom;
        else if (input == "nloci") file >> nloci;
        else if (input == "effect") file >> effect;
        else if (input == "xmax") file >> xmax;
        else if (input == "ymax") file >> ymax;
        else if (input == "tradeoff") file >> tradeoff;
        else if (input == "selfing") file >> selfing;
        else if (input == "recombination") file >> recombination;
        else if (input == "tend") file >> tend;
        else if (input == "tsave") file >> tsave;
        else if (input == "seed") file >> seed;
        else if (input == "loadarch") file >> loadarch;
        else if (input == "savepars") file >> savepars;
        else if (input == "savelog") file >> savelog;
        else if (input == "savearch") file >> savearch;
        else if (input == "talkative") file >> talkative;
        else if (input == "choose") file >> choose;
        else
            throw std::runtime_error("Invalid parameter name: " + input);

    }

    // Now update interactive parameters
    update();

    std::cout << "Parameters were read in succesfully.\n";

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

    if (type == 0u || type > 2u) throw std::runtime_error("Invalid type");
    if (popsize == 0u) throw std::runtime_error("Initial population size cannot be zero");
    for (size_t i = 0u; i < pgood.size(); ++i)
        if (pgood[i] < 0.0 || pgood[i] > 1.0)
            throw std::runtime_error("Proportion of good patches should be between zero and one");
    for (size_t i = 0u; i < 2u; ++i) {
        if (maxgrowths[i] < 0.0) throw std::runtime_error("Maximum growth rate cannot be negative");
        if (zwidths[i] < 0.0) throw std::runtime_error("Niche width cannot be negative");
        if (capacities[i] < 0.0) throw std::runtime_error("Carrying capacity cannot be negative");
        if (stress[i] < 0.0) throw std::runtime_error("Stress level cannot be negative");
    }
    if (growth < 0.0) throw std::runtime_error("Baseline growth rate cannot be negative");
    if (steep < 0.0) throw std::runtime_error("Steepness of the tolerance function cannot be negative");
    if (dispersal < 0.0 || dispersal > 1.0) throw std::runtime_error("Dispersal rate should be between zero and one");
    if (mutation < 0.0 || mutation > 1.0) throw std::runtime_error("Mutation rate should be between zero and one");
    if (nloci > 1000) throw std::runtime_error("There cannot be more than 1000 loci");
    if (nchrom == 0) throw std::runtime_error("There cannot be zero chromosomes");
    if (nloci == 0u) throw std::runtime_error("There cannot be zero loci");
    if (xmax < 0.0) throw std::runtime_error("Maximum tolerance cannot be negative");
    if (ymax < 0.0) throw std::runtime_error("Maximum competitiveness cannot be negative");
    if (tradeoff < 0.0) throw std::runtime_error("Trade-off cannot be negative");
    if (selfing < 0.0 || selfing > 1.0) throw std::runtime_error("Rate of selfing must be between zero and one");
    if (recombination < 0.0) throw std::runtime_error("Recombination rate cannot be negative");
    if (tend == 0u) throw std::runtime_error("Simulation time cannot be zero");
    if (tsave == 0u) throw std::runtime_error("Cannot save data every zero time point");

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

    file << "type " << type << '\n';
    file << "popsize " << popsize << '\n';
    file << "pgood " << pgood.size();
    for (size_t i = 0u; i < pgood.size(); ++i) file << ' ' << pgood[i];
    file << '\n';
    file << "maxgrowths " << maxgrowths[0u] << ' ' << maxgrowths[1u] << '\n';
    file << "zopts " << zopts[0u] << ' ' << zopts[1u] << '\n';
    file << "zwidths " << zwidths[0u] << ' ' << zwidths[1u] << '\n';
    file << "capacities " << capacities[0u] << ' ' << capacities[1u] << '\n';
    file << "stress " << stress[0u] << ' ' << stress[1u] << '\n';
    file << "growth " << growth << '\n';
    file << "steep " << steep << '\n';
    file << "dispersal " << dispersal << '\n';
    file << "mutation " << mutation << '\n';
    file << "nchrom " << nchrom << '\n';
    file << "nloci " << nloci << '\n';
    file << "effect " << effect << '\n';
    file << "xmax " << xmax << '\n';
    file << "ymax " << ymax << '\n';
    file << "tradeoff " << tradeoff << '\n';
    file << "selfing " << selfing << '\n';
    file << "recombination " << recombination << '\n';
    file << "tend " << tend << '\n';
    file << "tsave " << tsave << '\n';
    file << "seed " << seed << '\n';
    file << "loadarch " << loadarch << '\n';
    file << "savepars " << savepars << '\n';
    file << "savelog " << savelog << '\n';
    file << "savearch " << savearch << '\n';
    file << "talkative " << talkative << '\n';
    file << "choose " << choose << '\n';

}
