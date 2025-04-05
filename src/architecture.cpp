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

    // Check
    check();

    // Read in architecture if needed
    if (filename != "") read(filename);
    
}

// Function to check architecture parameters
void Architecture::check() const {

    // Scalar checks
    assert(nloci >= 0u);
    assert(locations.size() == nloci);
    assert(effects.size() == locations.size());
    assert(locations[0u] >= 0.0);
    assert(locations.back() <= 1.0);

    // Locations in increasing order
    for (size_t i = 0u; i < locations.size() - 1u; ++i)
        assert(locations[i + 1u] > locations[i]);

    // Check
    assert(!effects.empty());

    // All effect sizes are positive
    for (auto &effect : effects) assert(effect > 0.0);

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

    // Check the number of locations
    assert(locations.size() == nloci);

    // Check increasing order
    for (size_t l = 1u; l < nloci; ++l) assert(locations[l] > locations[l - 1u]);
    
    // Check bounds
    assert(locations[0u] >= 0.0);
    assert(locations.back() <= 1.0);

    // Check
    assert(!effects.empty());
    
    // Compute maximum trait value
    for (double &effect : effects) tolmax += effect;

}

// Function to load the genetic architecture from a file
void Architecture::read(const std::string &filename) {

    // filename: name of the file to read from

    // Open the architecture file
    std::ifstream file(filename.c_str());

    // Check if the file is open
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);

    // Reset
    nloci = 0u;

    // Read the number of loci
    file >> nloci;

    // Error if could not read
    if (file.fail())
        throw std::runtime_error("Could not read the number of loci in architecture file");

    // Error if no loci
    if (nloci == 0)
         throw std::runtime_error("There must be at least one locus in architecture file");

    // Resize containers
    locations.resize(nloci);
    effects.resize(nloci);

    // For each locus...
    for (size_t l = 0u; l < nloci; ++l) {

        // Read the locus position
        file >> locations[l];

        // Check if could not read
        if (file.fail())
            throw std::runtime_error("Could not read the location of locus " + std::to_string(l + 1u) + " in architecture file");

        // Check order
        if (l > 0u && locations[l] <= locations[l - 1]) 
            throw std::runtime_error("Locus locations must be in strictly increasing order in architecture file");

    }

    // Check bounds
    if (locations[0u] < 0.0) throw std::runtime_error("Locus location must be positive in architecture file");
    if (locations.back() > 1.0) throw std::runtime_error("Locus location cannot be beyond the end of the genome");

    // Reset maximum trait value
    tolmax = 0.0;

    // For each locus...
    for (size_t l = 0u; l < nloci; ++l) {

        // Read effect size
        file >> effects[l];

        // Check if could not read
        if (file.fail())
            throw std::runtime_error("Could not read the effect size of locus " + std::to_string(l + 1u) + " in architecture file");

        // Check sign
        if (effects[l] < 0.0)
            throw std::runtime_error("Effect size of locus " + std::to_string(l + 1u) + " must be positive in architecture file");

        // Sum effect sizees
        tolmax += effects[l];

    }

    // Error if needed
    if (tolmax == 0.0) 
        throw std::runtime_error("Sum of effect sizes must be strictly positive in architecture file");

    // Check
    assert(tolmax > 0.0);

    // Close the file
    file.close();

    // Check sizes
    assert(locations.size() == nloci);
    assert(effects.size() == nloci);

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
    file << nloci << '\n';
    
    // Write the locations of loci
    for (size_t l = 0u; l < nloci; ++l) file << ' ' << locations[l];
    
    // End of line
    file << '\n';
    
    // Write the effect sizes
    for (size_t l = 0u; l < nloci; ++l) file << ' ' << effects[l];
    
    // End of line
    file << '\n';

    // Close the file
    file.close();
    
}