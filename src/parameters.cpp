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
Parameters::Parameters(const std::string &filename) :
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
    nonlinear(1.0),
    selfing(0.95),
    recombination(1.0),
    tend(10u),
    tsave(20u),
    tchange(100000),
    twarming(1),
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

    // filename: optional parameter input file

    // Check
    check();

    // Read from file if needed
    if (filename != "") read(filename);

}

// Function to check parameter values are valid
void Parameters::check() const {

    // Check that the parameter values are valid
    assert(popsize != 0u);
    assert(pgood.size() != 0u);
    assert(pgood.size() == pgoodEnd.size());
    for (size_t i = 0u; i < pgood.size(); ++i) {
        assert(pgood[i] >= 0.0);
        assert(pgood[i] <= 1.0);
        assert(pgoodEnd[i] >= 0.0);
        assert(pgoodEnd[i] <= 1.0);
    }
    for (size_t i = 0u; i < 2u; ++i) {
        assert(capacities[i] > 0.0);
        assert(capacitiesEnd[i] > 0.0);
        assert(stress[i] >= 0.0);
        assert(stressEnd[i] >= 0.0);
    }
    assert(maxgrowth >= 0.0);
    assert(steep >= 0.0);
    assert(dispersal >= 0.0);
    assert(dispersal <= 1.0);
    assert(mutation >= 0.0);
    assert(mutation <= 1.0);
    assert(nloci <= 1000u);
    assert(nchrom != 0u);
    assert(nloci != 0u);
    assert(allfreq >= 0.0);
    assert(allfreq <= 1.0);
    assert(tradeoff >= 0.0);
    assert(nonlinear > 0.0);
    assert(selfing >= 0.0);
    assert(selfing <= 1.0);
    assert(recombination >= 0.0);
    assert(tend != 0u);
    assert(tsave != 0u);

}

// Function to check if the next value could not be read
void checkfail(std::ifstream &file, const std::string &name) {

    // file: input file stream
    // name: name of the parameter being read in

    // Check if the next value could not be read
    if (file.fail())
        throw std::runtime_error("Could not read value for parameter: " + name);

}

// Function to read in the next parameter value(s) as a single double
void readin(std::ifstream &file, double &x, const std::string &name) {

    // file: input file stream
    // x: parameter value
    // name: name of the parameter being read in

    // Read in the parameter value
    file >> x;

    // Check that the read was successful
    checkfail(file, name);

}

// Function to read in the next parameter value(s) as a vector of doubles
void readin(std::ifstream &file, std::vector<double> &v, const size_t &n, std::string name) {

    // file: input file stream
    // v: vector to update
    // n: number of parameter values
    // name: name of the vector of parameters being read in

    // Resize the vector
    v.resize(n);

    // Read in the number of parameter values
    for (size_t i = 0u; i < n; ++i) {

        // Read in the parameter value
        file >> v[i];

        // Add index to the name if provided
        if (name != "") name += "[" + std::to_string(i) + "]";

        // Check that the read was successful
        checkfail(file, name);

    }
}

// Function to read in the next parameter value(s) as a single unsigned integer
void readin(std::ifstream &file, size_t &x, const std::string &name) {

    // file: input file stream
    // x: parameter value
    // name: name of the parameter being read in

    // Read in the parameter value
    file >> x;

    // Check that the read was successful
    checkfail(file, name);

}

// Function to read in the next parameter value(s) as a single integer
void readin(std::ifstream &file, int &x, const std::string &name) {

    // file: input file stream
    // x: parameter value
    // name: name of the parameter being read in

    // Read in the parameter value
    file >> x;

    // Check that the read was successful
    checkfail(file, name);

}

// Function to read in the next parameter value(s) as a single boolean
void readin(std::ifstream &file, bool &x, const std::string &name) {

    // file: input file stream
    // x: parameter value
    // name: name of the parameter being read in

    // Read in the parameter value
    file >> x;

    // Check that the read was successful
    checkfail(file, name);

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

        // Read the next entry as parameter value(s)
        if (input == "popsize") readin(file, popsize, "popsize");
        else if (input == "pgood") {
            
            readin(file, ndemes, "ndemes");
            readin(file, pgood, ndemes, "pgood");

        }
        else if (input == "pgoodEnd") readin(file, pgoodEnd, ndemes, "pgoodEnd");
        else if (input == "capacities") readin(file, capacities, 2u, "capacities");
        else if (input == "capacitiesEnd") readin(file, capacitiesEnd, 2u, "capacitiesEnd");
        else if (input == "stress") readin(file, stress, 2u, "stress");
        else if (input == "stressEnd") readin(file, stressEnd, 2u, "stressEnd");
        else if (input == "maxgrowth") readin(file, maxgrowth, "maxgrowth");
        else if (input == "steep") readin(file, steep, "steep");
        else if (input == "dispersal") readin(file, dispersal, "dispersal");
        else if (input == "mutation") readin(file, mutation, "mutation");
        else if (input == "nchrom") readin(file, nchrom, "nchrom");
        else if (input == "nloci") readin(file, nloci, "nloci");
        else if (input == "effect") readin(file, effect, "effect");
        else if (input == "allfreq") readin(file, allfreq, "allfreq");
        else if (input == "tradeoff") readin(file, tradeoff, "tradeoff");
        else if (input == "nonlinear") readin(file, nonlinear, "nonlinear");
        else if (input == "selfing") readin(file, selfing, "selfing");
        else if (input == "recombination") readin(file, recombination, "recombination");
        else if (input == "tend") readin(file, tend, "tend");
        else if (input == "tsave") readin(file, tsave, "tsave");
        else if (input == "tchange") readin(file, tchange, "tchange");
        else if (input == "twarming") readin(file, twarming, "twarming");
        else if (input == "seed") readin(file, seed, "seed");
        else if (input == "sow") readin(file, sow, "sow");
        else if (input == "loadarch") readin(file, loadarch, "loadarch");
        else if (input == "savepars") readin(file, savepars, "savepars");
        else if (input == "savelog") readin(file, savelog, "savelog");
        else if (input == "savearch") readin(file, savearch, "savearch");
        else if (input == "savedat") readin(file, savedat, "savedat");
        else if (input == "choose") readin(file, choose, "choose");
        else if (input == "verbose") readin(file, verbose, "verbose");
        else
            throw std::runtime_error("Invalid parameter name: " + input);

    }

    // TODO: Make sure to say in the documentation that pgoodEnd must be entered after pgood
    // TODO: Make sure to say also that pgoodEnd must be manually set to the same size as pgood

    // Close the file
    file.close();

    // Check that the parameter values are valid
    if (popsize == 0u) throw std::runtime_error("Initial population size must be strictly positive");
    if (pgood.size() == 0u) throw std::runtime_error("Number of demes must be strictly positive");
    assert(pgood.size() == pgoodEnd.size()); // should have been caught already
    for (size_t i = 0u; i < pgood.size(); ++i) {
        if (pgood[i] < 0.0) throw std::runtime_error("Proportion of good patches must be between zero and one");
        if (pgood[i] > 1.0) throw std::runtime_error("Proportion of good patches must be between zero and one");
        if (pgoodEnd[i] < 0.0) throw std::runtime_error("Proportion of good patches after warming must be between zero and one");
        if (pgoodEnd[i] > 1.0) throw std::runtime_error("Proportion of good patches after warming must be between zero and one");
    }
    for (size_t i = 0u; i < 2u; ++i) {
        if (capacities[i] <= 0.0) throw std::runtime_error("Carrying capacity must be strictly positive");
        if (capacitiesEnd[i] <= 0.0) throw std::runtime_error("Carrying capacity after warming must be strictly positive");
        if (stress[i] < 0.0) throw std::runtime_error("Stress level must be positive");
        if (stressEnd[i] < 0.0) throw std::runtime_error("Stress level after warming must be positive");
    }
    if (maxgrowth < 0.0) throw std::runtime_error("Maximum growth rate must be positive");
    if (steep < 0.0) throw std::runtime_error("Steepness of the tolerance function must be positive");
    if (dispersal < 0.0) throw std::runtime_error("Dispersal rate must be between zero and one");
    if (dispersal > 1.0) throw std::runtime_error("Dispersal rate must be between zero and one");
    if (mutation < 0.0) throw std::runtime_error("Mutation rate must be between zero and one");
    if (mutation > 1.0) throw std::runtime_error("Mutation rate must be between zero and one");
    if (nloci > 1000) throw std::runtime_error("Number of loci must be 1000 at most");
    if (nchrom == 0) throw std::runtime_error("Number of chromosomes must be stricly positive");
    if (nloci == 0u) throw std::runtime_error("Number of loci must be stricly positive");
    if (allfreq < 0.0) throw std::runtime_error("Initial allele frequency must be between zero and one");
    if (allfreq > 1.0) throw std::runtime_error("Initial allele frequency must be between zero and one");
    if (tradeoff < 0.0) throw std::runtime_error("Trade-off strength must be positive");
    if (nonlinear <= 0.0) throw std::runtime_error("Non-linearity parameter must be strictly positive");
    if (selfing < 0.0) throw std::runtime_error("Rate of selfing must be between zero and one");
    if (selfing > 1.0) throw std::runtime_error("Rate of selfing must be between zero and one");
    if (recombination < 0.0) throw std::runtime_error("Recombination rate must be positive");
    if (tend == 0u) throw std::runtime_error("Simulation time must be strictly positive");
    if (tsave == 0u) throw std::runtime_error("Data saving frequency must be strictly positive");
    
}

// Save parameters to a file
void Parameters::save(const std::string &filename) const
{

    // filename: name of the file to save into

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
    file << "nonlinear " << nonlinear << '\n';
    file << "selfing " << selfing << '\n';
    file << "recombination " << recombination << '\n';
    file << "tend " << tend << '\n';
    file << "tsave " << tsave << '\n';
    file << "tchange " << tchange << '\n';
    file << "twarming " << twarming << '\n';
    file << "seed " << seed << '\n';
    file << "sow " << sow << '\n';
    file << "loadarch " << loadarch << '\n';
    file << "savepars " << savepars << '\n';
    file << "savelog " << savelog << '\n';
    file << "savearch " << savearch << '\n';
    file << "savedat " << savedat << '\n';
    file << "choose " << choose << '\n';
    file << "verbose " << verbose << '\n';

    // Close the file
    file.close();

}
