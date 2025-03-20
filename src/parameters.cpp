// This script contains all the functions of the Parameters structure.

#include "parameters.hpp"

// Create a default seed based on clock
size_t clockseed() {

    // Use the current time to generate a seed
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    // Convert the seed to integer and return
    return static_cast<size_t>(seed);
}

// Constructor
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
    tend(10u),
    tsave(20u),
    tchange(100000),
    twarming(1),
    type(1u),
    seed(clockseed()),
    sow(true),
    loadarch(false),
    savepars(false),
    savelog(false),
    savearch(true),
    savedat(false),
    choose(false),
    verbose(false)
{

    // Check that the parameter values are valid
    check();

}

// Read parameters from a file
void Parameters::read(const std::string &filename)
{

    // filename: name of the file to read parameters from

    // Open input file stream
    std::ifstream file(filename.c_str());

    // Check that the file is open
    if (!file.is_open()) 
        throw std::runtime_error("Unable to open file " + filename);

    // Prepare to read in
    std::string input;

    // We need to know the number of demes
    size_t ndemes = pgood.size();

    // For each line in the file...
    while (file >> input) {

        // Note: this loop conditions itself on the success of reading the input.
        // No need to check for failure.

        // Read in the parameter value(s)
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
        else if (input == "savedat") file >> savedat;
        else if (input == "choose") file >> choose;
        else if (input == "verbose") file >> verbose;
        else
            throw std::runtime_error("Invalid parameter name: " + input);

    }

    // Close the file
    file.close();

    // Check that the parameter values are valid
    check();

    // Verbose
    std::cout << "Parameters were read in succesfully.\n";

}

// Function to linearly update a parameter value upon time step increment
double lincrement(const double &x, const double &xfinal, const int &t, const int &tfinal) {

    // x: current value of the parameter
    // xfinal: final value the parameter is supposed to reach
    // t: current time step
    // tfinal: time step when the parameter should reach its final value

    // Compute new value
    return xfinal - x / (tfinal - t - 1.0);

    // Note: this follows from the equation of a line.

}

// Update climate-related parameters
void Parameters::update(const int &t) {

    // t: time step

    // Early exit if warming has not started
    if (t <= tchange) return;

    // Time at which climate change ends
    const int tfinal = tchange + twarming;

    // Early exit if warming is over
    if (t > tfinal) return;

    // For each patch...
    for (size_t i = 0u; i < 2u; ++i) {

        // Update the stress level and carrying capacity
        stress[i] = lincrement(stress[i], stressEnd[i], t, tfinal);
        capacities[i] = lincrement(capacities[i], capacitiesEnd[i], t, tfinal);

        // Check that they are still positive
        assert(stress[i] >= 0.0);
        assert(capacities[i] >= 0.0);

    }

    // For each deme...
    for (size_t i = 0u; i < pgood.size(); ++i) {

        // Update the cover of good patches
        pgood[i] = lincrement(pgood[i], pgoodEnd[i], t, tfinal);

        // Make sure it is still between zero and one
        assert(pgood[i] >= 0.0);
        assert(pgood[i] <= 1.0);

    }
}

// Check that the parameter values are valid
void Parameters::check() const
{

    // Check that the parameter values are valid
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

    // Parameter output file name
    const std::string filename = "paramlog.txt";

    // Create output file stream
    std::ofstream file(filename);
    
    // Check if the file is open
    if (!file.is_open()) 
        throw std::runtime_error("Unable to open file " + filename);

    // Write parameters to the file
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
    file << "savedat " << savedat << '\n';
    file << "choose " << choose << '\n';
    file << "talkative " << verbose << '\n';

    // Close the file
    file.close();

}
