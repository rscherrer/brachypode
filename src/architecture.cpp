// This script contains all the functions of the Architecture structure.

#include "architecture.hpp"

// Constructor
Architecture::Architecture(const size_t &nchrom, const size_t& nloci, const double &effect) :
    chromends(std::vector<double>(nchrom, 0.0)),
    locations(std::vector<double>(nloci, 0.0)),
    effects(std::vector<double>(nloci, effect))
{

    // nchrom: number of chromosomes
    // nloci: number of loci
    // effect: effect size per locus

    // For each chromosome...
    for (size_t k = 0u; k < nchrom; ++k) {

        // Set the end of the chromosome
        chromends[k] = (k + 1.0) / nchrom;

        // Note: chromosomes have equal length.

        // Check bounds
        assert(chromends[k] >= 0.0);
        assert(chromends[k] <= 1.0);
    }

    // Extra check
    assert(chromends.size() == nchrom);

    // Prepare a location sampler
    auto getlocation = rnd::uniform(0.0, 1.0);

    // Sample locus locations
    for (size_t l = 0u; l < nloci; ++l)
        locations[l] = getlocation(rnd::rng);

    // Now sort the vector of locations
    std::sort(locations.begin(), locations.end());

    // Check the number of locations
    assert(locations.size() == nloci);

    // For each locus...
    for (size_t l = 1u; l < nloci; ++l) {

        // Check its location
        assert(locations[l] > locations[l - 1u]);
        assert(locations[l] <= 1.0);
        assert(locations[l] >= 0.0);
    }

}

// Function to load the genetic architecture from a file
void Architecture::load() {

    // Name of the architecture file
    const std::string filename = "architecture.txt";

    // Open the architecture file
    std::ifstream file(filename.c_str());

    // Check if the file is open
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    // Prepare to read parameters
    std::string field;
    size_t nchrom = 1u;
    size_t nloci = 0u;

    // Read the number of chromosomes
    file >> nchrom;

    // Error if no chromosome
    if (nchrom == 0u) throw std::runtime_error("There should be at least one chromosome");

    // Resize container
    chromends.resize(nchrom);

    // For each chromosome...
    for (size_t k = 0u; k < nchrom; ++k) {

        // Read end of chromosome
        file >> chromends[k];

        // Check order
        if (k > 0u && chromends[k] <= chromends[k - 1u])
            throw std::runtime_error("Chromosome ends should be in increasing order");

        // Check bounds
        if (chromends[k] < 0.0) throw std::runtime_error("Chromosome ends should be positive");

    }

    // Check end of the last chromosome
    if (chromends.back() != 1.0) throw std::runtime_error("End of the last chromosome should be one");

    // Check size
    assert(chromends.size() == nchrom);

    // Read the number of loci
    file >> nloci;

    // Error if no loci
    if (nloci == 0) throw std::runtime_error("There should be at least one locus");

    // Resize containers
    locations.resize(nloci);
    effects.resize(nloci);

    // For each locus...
    for (size_t l = 0u; l < nloci; ++l) {

        // Read the locus position
        file >> locations[l];

        // Check order
        if (l > 0u && locations[l] <= locations[l - 1]) throw std::runtime_error("Locus locations should be in increasing order");

        // Check location
        if (locations[l] < 0.0) throw std::runtime_error("Locus location should be positive");
        if (locations[l] > chromends.back()) throw std::runtime_error("Locus location should not be beyond the end of the last chromosome");

    }

    // Read each effect size
    for (size_t l = 0u; l < nloci; ++l) file >> effects[l];

    // Close the file
    file.close();

}

// Function to save the genetic architecture to a file
void Architecture::save() const
{

    // Name of the architecture file
    const std::string filename = "architecture.txt";

    // Open the output architecture file
    std::ofstream archfile(filename);

    // Check if the file is open
    if (!archfile.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    // Number of chromosomes and loci
    const size_t nchrom = chromends.size();
    const size_t nloci = locations.size();

    // Write the number of chromosomes
    archfile << nchrom << '\n';

    // Write the ends of chromosomes
    for (size_t k = 0u; k < nchrom; ++k) archfile << ' ' << chromends[k];

    // End of line
    archfile << '\n';

    // Write the number of loci
    archfile << nloci << '\n';
    
    // Write the locations of loci
    for (size_t l = 0u; l < nloci; ++l) archfile << ' ' << locations[l];
    
    // End of line
    archfile << '\n';
    
    // Write the effect sizes
    for (size_t l = 0u; l < nloci; ++l) archfile << ' ' << effects[l];
    
    // End of line
    archfile << '\n';

    // Close the file
    archfile.close();
}
