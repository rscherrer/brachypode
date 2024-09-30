// This script contains all the functions of the Parameters structure.

#include "parameters.h"

Parameters::Parameters() :
    popsize(10u),
    pgood({0.8, 0.6, 0.5, 0.3, 0.1}),
    pgoodEnd({0.8, 0.6, 0.5, 0.3, 0.1}),
    stress({4.0, 0.0}),
    stressEnd({4.0, 0.0}),
    capacities({100.0, 10000.0}),
    capacitiesEnd({100.0, 10000.0}),
    maxgrowth(4.0),
    steep(2.0),
    dispersal(0.01),
    mutation(0.0001),
    nchrom(1u),
    nloci(50u),
    effect(0.1),
    allfreq(0.5),
    tradeoff(0.1),
    selfing(0.95),
    recombination(1.0),
    tend(100u),
    tsave(20u),
    tchange(100000),
    twarming(1),
    type(1u),
    seed(makeDefaultSeed()),
    sow(true),
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
    size_t ndemes = pgood.size();

    while (file >> input) {

        if (input == "popsize") file >> popsize;
        else if (input == "pgood") {
            file >> ndemes;
            if (!ndemes) throw std::runtime_error("Zero deme provided");
            pgood = std::vector<double>(ndemes);
            for (size_t i = 0u; i < ndemes; ++i)
                file >> pgood[i];
        }
        else if (input == "pgoodEnd") {
            pgoodEnd = std::vector<double>(ndemes);
            for (size_t i = 0u; i < ndemes; ++i)
                file >> pgoodEnd[i];
        }
        else if (input == "capacities") for (size_t i = 0u; i < 2u; ++i) file >> capacities[i];
        else if (input == "capacitiesEnd") for (size_t i = 0u; i < 2u; ++i) file >> capacitiesEnd[i];
        else if (input == "stress") for (size_t i = 0u; i < 2u; ++i) file >> stress[i];
        else if (input == "stressEnd") for (size_t i = 0u; i < 2u; ++i) file >> stressEnd[i];
        else if (input == "maxgrowth") file >> maxgrowth;
        else if (input == "steep") file >> steep;
        else if (input == "dispersal") file >> dispersal;
        else if (input == "mutation") file >> mutation;
        else if (input == "nchrom") file >> nchrom;
        else if (input == "nloci") file >> nloci;
        else if (input == "effect") file >> effect;
        else if (input == "allfreq") file >> allfreq;
        else if (input == "tradeoff") file >> tradeoff;
        else if (input == "selfing") file >> selfing;
        else if (input == "recombination") file >> recombination;
        else if (input == "tend") file >> tend;
        else if (input == "tsave") file >> tsave;
        else if (input == "tchange") file >> tchange;
        else if (input == "twarming") file >> twarming;
        else if (input == "type") file >> type;
        else if (input == "seed") file >> seed;
        else if (input == "sow") file >> sow;
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

// Function to update a parameter value during climate change
void updatePar(double &x, const double &xend, const double &dt) {

    const double dx = dt == 0.0 ? 0.0 : (xend - x) / dt;
    x += dx;

}

// Update climate-related parameters
void Parameters::changeClimate(const double &dt) {

    assert(dt >= 0.0);

    for (size_t i = 0u; i < pgood.size(); ++i) updatePar(pgood[i], pgoodEnd[i], dt);
    for (size_t i = 0u; i < 2u; ++i) {
        updatePar(stress[i], stressEnd[i], dt);
        updatePar(capacities[i], capacitiesEnd[i], dt);
    }

}

// Check that the parameter values are valid
void Parameters::check() const
{
    std::string msg = "No error detected";

    if (popsize == 0u) throw std::runtime_error("Initial population size cannot be zero");
    for (size_t i = 0u; i < pgood.size(); ++i) {
        if (pgood[i] < 0.0 || pgood[i] > 1.0) throw std::runtime_error("Proportion of good patches should be between zero and one");
        if (pgoodEnd[i] < 0.0 || pgoodEnd[i] > 1.0) throw std::runtime_error("Proportion of good patches after warming should be between zero and one");
    }
    for (size_t i = 0u; i < 2u; ++i) {
        if (capacities[i] < 0.0) throw std::runtime_error("Carrying capacity cannot be negative");
        if (capacitiesEnd[i] < 0.0) throw std::runtime_error("Carrying capacity after warming cannot be negative");
        if (stress[i] < 0.0) throw std::runtime_error("Stress level cannot be negative");
        if (stressEnd[i] < 0.0) throw std::runtime_error("Stress level after warming cannot be negative");
    }
    if (maxgrowth < 0.0) throw std::runtime_error("Maximum growth rate cannot be negative");
    if (steep < 0.0) throw std::runtime_error("Steepness of the tolerance function cannot be negative");
    if (dispersal < 0.0 || dispersal > 1.0) throw std::runtime_error("dispersal rate should be between zero and one");
    if (mutation < 0.0 || mutation > 1.0) throw std::runtime_error("Mutation rate should be between zero and one");
    if (nloci > 1000) throw std::runtime_error("There cannot be more than 1000 loci");
    if (nchrom == 0) throw std::runtime_error("There cannot be zero chromosomes");
    if (nloci == 0u) throw std::runtime_error("There cannot be zero loci");
    if (allfreq < 0.0 || allfreq > 1.0) throw std::runtime_error("Initial allele frequency should be between zero and one");
    if (tradeoff < 0.0) throw std::runtime_error("Trade-off cannot be negative");
    if (selfing < 0.0 || selfing > 1.0) throw std::runtime_error("Rate of selfing must be between zero and one");
    if (recombination < 0.0) throw std::runtime_error("Recombination rate cannot be negative");
    if (tend == 0u) throw std::runtime_error("Simulation time cannot be zero");
    if (tsave == 0u) throw std::runtime_error("Cannot save data every zero time point");
    if (type == 0u || type > 2u) throw std::runtime_error("Type should be 1 or 2");
    if (type == 2u && tradeoff > 1.0) throw std::runtime_error("Trade-off should be between 0 and 1 if type is 2");

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
    file << "pgood " << pgood.size();
    for (size_t i = 0u; i < pgood.size(); ++i) file << ' ' << pgood[i];
    file << '\n';
    file << "pgoodEnd";
    for (size_t i = 0u; i < pgoodEnd.size(); ++i) file << ' ' << pgoodEnd[i];
    file << '\n';
    file << "capacities " << capacities[0u] << ' ' << capacities[1u] << '\n';
    file << "capacitiesEnd " << capacitiesEnd[0u] << ' ' << capacitiesEnd[1u] << '\n';
    file << "stress " << stress[0u] << ' ' << stress[1u] << '\n';
    file << "stressEnd " << stressEnd[0u] << ' ' << stressEnd[1u] << '\n';
    file << "maxgrowth " << maxgrowth << '\n';
    file << "steep " << steep << '\n';
    file << "dispersal " << dispersal << '\n';
    file << "mutation " << mutation << '\n';
    file << "nchrom " << nchrom << '\n';
    file << "nloci " << nloci << '\n';
    file << "effect " << effect << '\n';
    file << "allfreq " << allfreq << '\n';
    file << "tradeoff " << tradeoff << '\n';
    file << "selfing " << selfing << '\n';
    file << "recombination " << recombination << '\n';
    file << "tend " << tend << '\n';
    file << "tsave " << tsave << '\n';
    file << "tchange " << tchange << '\n';
    file << "twarming " << twarming << '\n';
    file << "type " << type << '\n';
    file << "seed " << seed << '\n';
    file << "sow " << sow << '\n';
    file << "loadarch " << loadarch << '\n';
    file << "savepars " << savepars << '\n';
    file << "savelog " << savelog << '\n';
    file << "savearch " << savearch << '\n';
    file << "talkative " << talkative << '\n';
    file << "choose " << choose << '\n';

}
