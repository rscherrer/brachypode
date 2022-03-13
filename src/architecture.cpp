#include "architecture.h"

Architecture::Architecture(const size_t &nchrom, const size_t& nloci) :
    chromends(std::vector<double>(nchrom, 0.0)),
    locations(std::vector<double>(nloci, 0.0))
{

    // Generate ends of chromosomes (chromosomes have equal sizes)
    for (size_t k = 0u; k < nchrom; ++k) {
        chromends[k] = (k + 1.0) / nchrom;
        assert(chromends[k] >= 0.0);
        assert(chromends[k] <= 1.0);
    }

    // Extra check
    assert(chromends.size() == nchrom);

    // Sample random gene locations
    auto getlocation = rnd::uniform(0.0, 1.0);

    for (size_t l = 0u; l < nloci; ++l)
        locations[l] = getlocation(rnd::rng);

    // Now sort the vector of locations
    std::sort(locations.begin(), locations.end());

    // Check the locus locations
    assert(locations.size() == nloci);
    for (size_t l = 1u; l < nloci; ++l) {
        assert(locations[l] > locations[l - 1u]);
        assert(locations[l] <= 1.0);
        assert(locations[l] >= 0.0);
    }

}

void Architecture::load() {

    const std::string filename = "architecture.txt";

    // Open the architecture file
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    // Prepare to read parameters
    std::string field;
    size_t nchrom = 1u;
    size_t nloci = 0u;

    // Read the number of chromosomes
    file >> nchrom;
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
        if (chromends[k] < 0.0) throw std::runtime_error("Chromosome ends should be postitive");

    }

    // Check end of the last chromosome
    if (chromends.back() != 1.0) throw std::runtime_error("End of the last chromosome should be one");

    // Check size
    assert(chromends.size() == nchrom);

    // Read the number of loci
    file >> nloci;
    if (nloci == 0) throw std::runtime_error("There should be at least one locus");

    // Resize containers
    locations.resize(nloci);

    // For each locus...
    for (size_t l = 0u; l < nloci; ++l) {

        // Read the locus position
        file >> locations[l];

        // Check location
        if (locations[l] < 0.0) throw std::runtime_error("Locus location should be positive");
        if (locations[l] > chromends.back()) throw std::runtime_error("Locus location should not be beyond the end of the last chromosome");

    }

    file.close();

}

void Architecture::save(const Parameters &pars) const
{
    const std::string filename = "architecture.txt";
    std::ofstream archfile(filename);

    if (!archfile.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    // Write chromosome ends
    assert(pars.nchrom == chromends.size());
    archfile << pars.nchrom;
    for (size_t k = 0u; k < pars.nchrom; ++k) archfile << ' ' << chromends[k];

    // Write locus locations
    assert(pars.nloci == locations.size());
    archfile << pars.nloci;
    for (size_t l = 0u; l < pars.nloci; ++l) archfile << ' ' << locations[l];

    archfile.close();
}
