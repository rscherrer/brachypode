// This script contains all the functions of the Architecture structure.

#include "architecture.hpp"

// TODO: Say in the documentation that architecture loading will overwrite parameters

// Constructor
Architecture::Architecture(const Parameters &pars, const std::string &filename) :
    nchrom(pars.nchrom),
    nloci(pars.nloci),
    tolmax(pars.effect * pars.nloci),
    chromends(std::vector<double>(nchrom, 0.0)),
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

    // Scalar checks
    assert(nchrom > 0u);
    assert(nloci >= 0u);
    assert(chromends.size() == nchrom);
    assert(locations.size() == nloci);
    assert(effects.size() == locations.size());
    assert(chromends[0u] > 0.0);
    assert(chromends.back() == 1.0);
    assert(locations[0u] >= 0.0);
    assert(locations.back() <= 1.0);

    // Chromosome ends in increasing order
    for (size_t i = 0u; i < chromends.size() - 1u; ++i)
        assert(chromends[i + 1u] > chromends[i]);

    // Locations in increasing order
    for (size_t i = 0u; i < locations.size() - 1u; ++i)
        assert(locations[i + 1u] > locations[i]);

}

// Function to generate a new architecture
void Architecture::make() {

    // effect: locus effect size

    // For each chromosome...
    for (size_t k = 0u; k < nchrom; ++k) {

        // Set the end of the chromosome
        chromends[k] = (k + 1.0) / nchrom;

        // Note: chromosomes have equal length.

        // Check increasing order
        if (k > 0u) assert(chromends[k] > chromends[k - 1u]);

    }

    // Extra checks
    assert(chromends.size() == nchrom);
    assert(chromends[0u] > 0.0);
    assert(chromends.back() == 1.0);

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
    assert(locations.back() <= chromends.back());

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

    // Reset hyperparameters
    nchrom = 1u;
    nloci = 0u;

    // Read the number of chromosomes
    file >> nchrom;

    // Error if could not read
    if (file.fail())
        throw std::runtime_error("Could not read the number of chromosomes in architecture file");

    // Error if no chromosome
    if (nchrom == 0u) 
        throw std::runtime_error("There must be at least one chromosome in architecture file");

    // Resize container
    chromends.resize(nchrom);

    // For each chromosome...
    for (size_t k = 0u; k < nchrom; ++k) {

        // Read end of chromosome
        file >> chromends[k];

        // Check if could not read
        if (file.fail())
            throw std::runtime_error("Could not read the end of chromosome " + std::to_string(k) + " in architecture file");

        // Check order
        if (k > 0u && chromends[k] <= chromends[k - 1u])
            throw std::runtime_error("Chromosome ends must be in strictly increasing order in architecture file");

    }

    // Check end of the first chromosome
    if (chromends[0u] < 0.0)
        throw std::runtime_error("Chromosome ends must be strictly positive in architecture file");

    // Check end of the last chromosome
    if (chromends.back() != 1.0) 
        throw std::runtime_error("End of the last chromosome must be one in architecture file");

    // Check size
    assert(chromends.size() == nchrom);

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
            throw std::runtime_error("Could not read the location of locus " + std::to_string(l) + " in architecture file");

        // Check order
        if (l > 0u && locations[l] <= locations[l - 1]) 
            throw std::runtime_error("Locus locations must be in strictly increasing order in architecture file");

    }

    // Check bounds
    if (locations[0u] < 0.0) throw std::runtime_error("Locus location must be positive in architecture file");
    if (locations.back() > chromends.back()) throw std::runtime_error("Locus location cannot be beyond the end of the last chromosome in architecture file");

    // Read each effect size
    for (size_t l = 0u; l < nloci; ++l) file >> effects[l];

    // Close the file
    file.close();

    // Check sizes
    assert(locations.size() == nloci);
    assert(effects.size() == nloci);

    // Reset maximum trait value
    tolmax = 0.0;

    // Re-compute maximum trait value
    for (double &effect : effects) tolmax += effect;

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

    // Write the number of chromosomes
    file << nchrom << '\n';

    // Write the ends of chromosomes
    for (size_t k = 0u; k < nchrom; ++k) file << ' ' << chromends[k];

    // End of line
    file << '\n';

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