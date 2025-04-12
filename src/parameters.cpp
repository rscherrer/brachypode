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
    assert(chk::isstrictpos(popsize != 0u));
    assert(chk::isstrictpos(ndemes));
    assert(pgood.size() == ndemes);
    assert(pgood.size() == pgoodEnd.size());
    for (auto &p : pgood) assert(chk::isproportion(p));
    for (auto &p : pgoodEnd) assert(chk::isproportion(p));
    for (auto &x : capacities) assert(chk::isstrictpos(x));
    for (auto &x : capacitiesEnd) assert(chk::isstrictpos(x));
    for (auto &x : stress) assert(chk::ispositive(x));
    for (auto &x : stressEnd) assert(chk::ispositive(x));
    assert(chk::ispositive(maxgrowth));
    assert(chk::ispositive(steep));
    assert(chk::isproportion(dispersal));
    assert(chk::isproportion(mutation));
    assert(chk::isproportion(nloci / 1000.0));
    assert(chk::isstrictpos(effect));
    assert(chk::isproportion(allfreq));
    assert(chk::ispositive(tradeoff));
    assert(chk::isstrictpos(nonlinear));
    assert(chk::isproportion(selfing));
    assert(chk::ispositive(recombination));
    assert(chk::isstrictpos(minrealk));
    assert(chk::isenoughmb(memsave));
    assert(chk::isstrictpos(tend));
    assert(chk::isstrictpos(tsave));
    assert(chk::isstrictpos(twarming));

}

// Read parameters from a file
void Parameters::read(const std::string &filename)
{

    // filename: name of the file to read parameters from

    // Create a reader
    Reader reader(filename);

    // Open it
    reader.open();

    // TODO: Move reader header call here

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
        if (name == "popsize") reader.readvalue(popsize, "strictpos");
        else if (name == "ndemes") reader.readvalue(ndemes, "strictpos");
        else if (name == "pgood") reader.readvalues(pgood, ndemes, "proportion");
        else if (name == "pgoodEnd") reader.readvalues(pgoodEnd, ndemes, "proportion");
        else if (name == "capacities") reader.readvalues(capacities, 2u, "strictpos");
        else if (name == "capacitiesEnd") reader.readvalues(capacitiesEnd, 2u, "strictpos");
        else if (name == "stress") reader.readvalues(stress, 2u, "positive");
        else if (name == "stressEnd") reader.readvalues(stressEnd, 2u, "positive");
        else if (name == "maxgrowth") reader.readvalue(maxgrowth, "positive");
        else if (name == "steep") reader.readvalue(steep, "positive");
        else if (name == "dispersal") reader.readvalue(dispersal, "proportion");
        else if (name == "mutation") reader.readvalue(mutation, "proportion");
        else if (name == "nloci") reader.readvalue(nloci, "onetothousand");
        else if (name == "effect") reader.readvalue(effect, "strictpos");
        else if (name == "allfreq") reader.readvalue(allfreq, "proportion");
        else if (name == "tradeoff") reader.readvalue(tradeoff, "positive");
        else if (name == "nonlinear") reader.readvalue(nonlinear, "strictpos");
        else if (name == "selfing") reader.readvalue(selfing, "proportion");
        else if (name == "recombination") reader.readvalue(recombination, "positive");
        else if (name == "minrealk") reader.readvalue(minrealk, "strictpos");
        else if (name == "memsave") reader.readvalue(memsave, "enoughmb");
        else if (name == "tend") reader.readvalue(tend, "strictpos");
        else if (name == "tsave") reader.readvalue(tsave, "strictpos");
        else if (name == "tchange") reader.readvalue(tchange);
        else if (name == "twarming") reader.readvalue(twarming, "strictpos");
        else if (name == "seed") reader.readvalue(seed);
        else if (name == "sow") reader.readvalue(sow);
        else if (name == "loadarch") reader.readvalue(loadarch);
        else if (name == "savepars") reader.readvalue(savepars);
        else if (name == "savearch") reader.readvalue(savearch);
        else if (name == "savedat") reader.readvalue(savedat);
        else if (name == "choose") reader.readvalue(choose);
        else if (name == "verbose") reader.readvalue(verbose);
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
