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

void read(std::vector<double> &v, const size_t &n, std::ifstream &file)
{
    for (size_t i = 0u; i < n; ++i)
        file >> v[i];
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

    // Read in parameters of interest first
    do {

        file >> field;

        if (field == "nchrom") file >> nchrom;
        if (field == "nloci") file >> nloci;

    }
    while (field != "--architecture--");

    // Reset the architecture and check dimensions
    chromends.resize(nchrom);
    locations.resize(nloci);

    assert(chromends.size() == nchrom);
    assert(locations.size() == nloci);

    // Read in architecture
    while (file >> field) {

        if (field == "chromends") read(chromends, nchrom, file);
        else if (field == "locations") read(locations, nloci, file);

    }

    file.close();

}

// Write vector as a row in text file, with end of line
void write(const std::vector<double> &v, std::ofstream &file)
{
    for (auto x : v)
        file << x << ' ';
    file << '\n';
}

void Architecture::save(const Parameters &pars) const
{
    const std::string filename = "architecture.txt";
    std::ofstream archfile(filename);

    if (!archfile.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    // Write parameters first
    archfile << "--parameters--\n";
    pars.write(archfile);

    archfile << "\n--architecture--\n";

    archfile << "chromends ";
    write(chromends, archfile);

    archfile << "locations ";
    write(locations, archfile);

    archfile.close();
}
