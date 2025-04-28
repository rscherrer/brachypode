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
    ndemes(3u),
    pgood({0.8, 0.8, 0.8}),
    pgoodEnd({0.1, 0.1, 0.1}),
    stress({4.0, 0.0}),
    stressEnd({4.0, 0.0}),
    capacities({100.0, 10000.0}),
    capacitiesEnd({100.0, 10000.0}),
    maxgrowth(4.0),
    steep(2.0),
    dispersal(0.01),
    mutation(0.0001),
    nloci(10u),
    effect(0.1),
    allfreq(0.1),
    tradeoff(0.1),
    nonlinear(1.0),
    selfing(0.95),
    recombination(1.0),
    minrealk(1E-06),
    memsave(1.0),
    tend(10u),
    tsave(20u),
    tchange(100000u),
    twarming(1u),
    seed(clockseed()),
    sow(false),
    loadarch(false),
    savepars(false),
    savearch(false),
    savedat(false),
    choose(false),
    verbose(false)
{

    // filename: optional parameter input file

    // Read from file if needed
    if (filename != "") read(filename);

}

// Function to check parameter values are valid
void Parameters::check() const {

    // Check that the parameter values are valid
    assert(popsize != 0u);
    assert(ndemes != 0u);
    assert(pgood.size() == ndemes);
    assert(pgoodEnd.size() == ndemes);
    assert(capacities.size() == 2u);
    assert(capacitiesEnd.size() == 2u);
    assert(stress.size() == 2u);
    assert(stressEnd.size() == 2u);
    for (auto p : pgood) assert(p >= 0.0 && p <= 1.0);
    for (auto p : pgoodEnd) assert(p >= 0.0 && p <= 1.0);
    for (auto x : capacities) assert(x > 0.0);
    for (auto x : capacitiesEnd) assert(x > 0.0);
    for (auto x : stress) assert(x >= 0.0);
    for (auto x : stressEnd) assert(x >= 0.0);
    assert(maxgrowth >= 0.0);
    assert(steep >= 0.0);
    assert(dispersal >= 0.0 && dispersal <= 1.0);
    assert(mutation >= 0.0 && mutation <= 1.0);
    assert(nloci > 0u && nloci <= 1000u);
    assert(effect > 0.0);
    assert(allfreq >= 0.0 && allfreq <= 1.0);
    assert(tradeoff >= 0.0);
    assert(nonlinear > 0.0);
    assert(selfing >= 0.0 && selfing <= 1.0);
    assert(recombination >= 0.0);
    assert(minrealk > 0.0);
    assert(memsave >= 0.0);
    assert(tend > 0u);
    assert(tsave > 0u);
    assert(twarming > 0u);

}

// Read parameters from a file
void Parameters::read(const std::string &filename)
{

    // filename: name of the file to read parameters from

    // Create a reader
    Reader reader(filename);

    // Open it
    reader.open();

    // For each line in the file...
    while (!reader.iseof()) {

        // Read a line
        reader.readline();

        // Skip empty line
        if (reader.isempty()) continue;

        // Skip if comment line
        if (reader.iscomment()) continue;

        // Check
        assert(!reader.isempty());
        assert(!reader.iscomment());
        
        // Current parameter name 
        std::string name = reader.getname();

        // Read the parameter value(s)
        if (name == "popsize") reader.readvalue<size_t>(popsize, chk::strictpos<size_t>);
        else if (name == "ndemes") reader.readvalue<size_t>(ndemes, chk::strictpos<size_t>);
        else if (name == "pgood") reader.readvalues<double>(pgood, ndemes, chk::proportion<double>);
        else if (name == "pgoodEnd") reader.readvalues<double>(pgoodEnd, ndemes, chk::proportion<double>);
        else if (name == "capacities") reader.readvalues<double>(capacities, 2u, chk::strictpos<double>);
        else if (name == "capacitiesEnd") reader.readvalues<double>(capacitiesEnd, 2u, chk::strictpos<double>);
        else if (name == "stress") reader.readvalues<double>(stress, 2u, chk::positive<double>);
        else if (name == "stressEnd") reader.readvalues<double>(stressEnd, 2u, chk::positive<double>);
        else if (name == "maxgrowth") reader.readvalue<double>(maxgrowth, chk::positive<double>);
        else if (name == "steep") reader.readvalue<double>(steep, chk::positive<double>);
        else if (name == "dispersal") reader.readvalue<double>(dispersal, chk::proportion<double>);
        else if (name == "mutation") reader.readvalue<double>(mutation, chk::proportion<double>);
        else if (name == "nloci") reader.readvalue<size_t>(nloci, chk::onetothousand<size_t>);
        else if (name == "effect") reader.readvalue<double>(effect, chk::strictpos<double>);
        else if (name == "allfreq") reader.readvalue<double>(allfreq, chk::proportion<double>);
        else if (name == "tradeoff") reader.readvalue<double>(tradeoff, chk::positive<double>);
        else if (name == "nonlinear") reader.readvalue<double>(nonlinear, chk::strictpos<double>);
        else if (name == "selfing") reader.readvalue<double>(selfing, chk::proportion<double>);
        else if (name == "recombination") reader.readvalue<double>(recombination, chk::positive<double>);
        else if (name == "minrealk") reader.readvalue<double>(minrealk, chk::strictpos<double>);
        else if (name == "memsave") reader.readvalue<double>(memsave, chk::enoughmb<double>);
        else if (name == "tend") reader.readvalue<size_t>(tend, chk::strictpos<size_t>);
        else if (name == "tsave") reader.readvalue<size_t>(tsave, chk::strictpos<size_t>);
        else if (name == "tchange") reader.readvalue<size_t>(tchange);
        else if (name == "twarming") reader.readvalue<size_t>(twarming, chk::strictpos<size_t>);
        else if (name == "seed") reader.readvalue<size_t>(seed);
        else if (name == "sow") reader.readvalue<bool>(sow);
        else if (name == "loadarch") reader.readvalue<bool>(loadarch);
        else if (name == "savepars") reader.readvalue<bool>(savepars);
        else if (name == "savearch") reader.readvalue<bool>(savearch);
        else if (name == "savedat") reader.readvalue<bool>(savedat);
        else if (name == "choose") reader.readvalue<bool>(choose);
        else if (name == "verbose") reader.readvalue<bool>(verbose);
        else 
            reader.readerror();

    }

    // Close the file
    reader.close();

    // Check
    check();

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
    file << "ndemes " << ndemes << '\n'; 
    file << "pgood ";
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
    file << "nloci " << nloci << '\n';
    file << "effect " << effect << '\n';
    file << "allfreq " << allfreq << '\n';
    file << "tradeoff " << tradeoff << '\n';
    file << "nonlinear " << nonlinear << '\n';
    file << "selfing " << selfing << '\n';
    file << "recombination " << recombination << '\n';
    file << "minrealk " << minrealk << '\n';
    file << "memsave " << memsave << '\n';
    file << "tend " << tend << '\n';
    file << "tsave " << tsave << '\n';
    file << "tchange " << tchange << '\n';
    file << "twarming " << twarming << '\n';
    file << "seed " << seed << '\n';
    file << "sow " << sow << '\n';
    file << "loadarch " << loadarch << '\n';
    file << "savepars " << savepars << '\n';
    file << "savearch " << savearch << '\n';
    file << "savedat " << savedat << '\n';
    file << "choose " << choose << '\n';
    file << "verbose " << verbose << '\n';

    // Close the file
    file.close();

}
