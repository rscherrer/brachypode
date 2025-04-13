// This script contains all the functions of the Architecture structure.

#include "architecture.hpp"

// Constructor
Architecture::Architecture(const Parameters &pars, const std::string &filename) :
    nloci(pars.nloci),
    tolmax(pars.effect * pars.nloci),
    locations(std::vector<double>(nloci, 0.0)),
    effects(std::vector<double>(nloci, pars.effect))
{

    // pars: architecture parameters
    // filename: optional architecture file to load

    // Generate a new architecture
    make();

    // Read in architecture if needed
    if (filename != "") read(filename);
    
}

// Function to check architecture parameters
void Architecture::check() const {

    // Check
    assert(nloci != 0u);
    assert(nloci <= 1000u);
    assert(locations.size() == nloci);
    assert(effects.size() == nloci);
    assert(locations[0u] >= 0.0);
    assert(locations.back() <= 1.0);
    for (size_t l = 1u; l < locations.size(); ++l)
        assert(locations[l] > locations[l - 1u]);
    assert(!effects.empty());
    assert(tolmax > 0.0);
    for (auto x : effects) assert(x > 0.0);

}

// Function to generate a new architecture
void Architecture::make() {

    // Prepare a location sampler
    auto getLocation = rnd::uniform(0.0, 1.0);

    // Sample locus locations
    for (size_t l = 0u; l < nloci; ++l)
        locations[l] = getLocation(rnd::rng);

    // Now sort the vector of locations
    std::sort(locations.begin(), locations.end());
    
    // Compute maximum trait value
    for (double &effect : effects) tolmax += effect;

    // Check
    check();

}

// Function to load the genetic architecture from a file
void Architecture::read(const std::string &filename) {

    // filename: name of the file to read from

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
        if (name == "nloci") reader.readvalue<size_t>(nloci, chk::onetothousand<size_t>);
        else if (name == "locations") reader.readvalues<double>(locations, nloci, chk::proportion<double>, chk::strictorder<double>);
        else if (name == "effects") reader.readvalues<double>(effects, nloci, chk::strictpos<double>);
        else 
            reader.readerror();

        // Check that we have reached the end of the line
        assert(reader.iseol());
 
    }

    // Check that we have reached the end of the file
    assert(reader.iseof());

    // Close the file
    reader.close();

    // Reset
    tolmax = 0.0;

    // Update
    for (double &effect : effects) tolmax += effect;

    // Check
    check();

}

// Function to save the genetic architecture to a file
void Architecture::save(const std::string &filename) const
{

    // filename: name of the file to save to

    // Open the output architecture file
    std::ofstream file(filename);

    // Check if the file is open
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);

    // Write the number of loci
    file << "nloci " << nloci << '\n';
    
    // Write the locations of loci
    file << "locations";
    for (size_t l = 0u; l < nloci; ++l) file << ' ' << locations[l];
    file << '\n';

    // Write the effect sizes
    file << "effects";
    for (size_t l = 0u; l < nloci; ++l) file << ' ' << effects[l];
    file << '\n';

    // Close the file
    file.close();
    
}