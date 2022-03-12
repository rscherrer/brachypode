#include "architecture.h"

Architecture::Architecture(const Parameters &p) :
    nchrom(p.nchrom),
    ntol(p.ntol),
    ncomp(p.ncomp),
    nneut(p.nneut),
    nloci(ntol + ncomp + nneut),
    chromends(std::vector<double>(nchrom)),
    locations(std::vector<double>(nloci)),
    chromosomes(std::vector<size_t>(nloci)),
    traits(std::vector<size_t>(nloci)),
    effects(std::vector<double>(nloci, p.effect))
{

    // Assign end locations to each chromosome
    for (size_t k = 0u; k < chromends.size(); ++k)
        chromends[k] = (k + 1.0) / chromends.size();

    // Sample locus relative locations along the genome
    auto getLocation = rnd::uniform(0.0, 1.0);

    for (size_t l = 0u; l < nloci; ++l)
        locations[l] = getLocation(rnd::rng);

    std::sort(locations.begin(), locations.end());

    // Assign chromosomes to each locus
    for (size_t l = 0u, k = 0u; l < nloci && k < p.nchrom; ) {
        if (locations[l] < chromends[k]) {
            chromosomes[l] = k;
            ++l;
        } else ++k;
    }

    // Randomly assign encoded trait to each locus
    size_t l = 0u;
    for (; l < p.ntol; ++l) traits[l] = 0u;
    for (; l < p.ntol + p.ncomp; ++l) traits[l] = 1u;
    for (; l < nloci; ++l) traits[l] = 2u;

    std::shuffle(traits.begin(), traits.end(), rnd::rng);

    assert(chromends.size() == p.nchrom);
    assert(locations.size() == nloci);
    assert(chromosomes.size() == nloci);
    assert(traits.size() == nloci);
    assert(effects.size() == nloci);

}

void Architecture::load() {

    // This function will overwrite the genetic architecture
    // with that found in the arhictecture file provided
    // and update the parameters accordingly

    // The file should contain the hyperparameters first (i.e. nchrom, nloci),
    // then the lists of parameter values (e.g. locations, effects...)

    const std::string filename = "architecture.txt";

    // Open the architecture file
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    std::string field;
    bool isNChrom = false, isNLoci = false;

    while (!(isNChrom || isNLoci)) {

        file >> field;

        if (field == "nchrom") { file >> nchrom; isNChrom = true; }
        else if (field == "nloci") { file >> nloci; isNLoci = true; }
        else throw std::runtime_error("Hyperparameters must be provided first");

    }

    assert(isNChrom && isNLoci);

    if (nchrom < 1u) throw std::runtime_error("There should be at least one chromosome");
    if (nloci < 3u) throw std::runtime_error("There should be at least three loci");

    chromends = std::vector<double>(nchrom);
    locations = std::vector<double>(nloci);
    chromosomes = std::vector<size_t>(nloci);
    traits = std::vector<size_t>(nloci);
    effects = std::vector<double>(nloci);

    bool isChromEnds = false, isLocations = false, isTraits = false, isEffects = false;

    while (file >> field) {

        if (field == "chromends") { isChromEnds = true; for (size_t k = 0u; k < nchrom; ++k) file >> chromends[k]; }
        else if (field == "locations") { isLocations = true; for (size_t l = 0u; l < nloci; ++l) file >> locations[l]; }
        else if (field == "traits") { isTraits = true; for (size_t l = 0u; l < nloci; ++l) file >> traits[l]; }
        else if (field == "effects") { isEffects = true; for (size_t l = 0u; l < nloci; ++l) file >> effects[l]; }
        else throw std::runtime_error("Unknown architecture field provided");

    }

    file.close();

    // Check that all fields were provided
    if (!(isChromEnds && isLocations && isTraits && isEffects))
        throw std::runtime_error("Some architecture fields are missing");

    // For each chromosome...
    for (size_t k = 0u; k < nchrom; ++k) {

        // Check that its end location is positive
        if (chromends[k] <= 0.0) throw std::runtime_error("Chromosome ends should be positive");

        // Check that the chromosomes are in increasing order
        if (k > 0u) if (chromends[k] <= chromends[k - 1u]) throw std::runtime_error("Chromosome ends should be in increasing order");

    }

    // Assign chromosomes to each locus
    for (size_t l = 0u, k = 0u; l < nloci && k < nchrom; ) {
        if (locations[l] < chromends[k]) {
            chromosomes[l] = k;
            ++l;
        } else ++k;
    }

    assert(chromends.size() == nchrom);
    assert(locations.size() == nloci);
    assert(chromosomes.size() == nloci);
    assert(traits.size() == nloci);
    assert(effects.size() == nloci);

    ntol = 0u, ncomp = 0u, nneut = 0u;

    // For each locus...
    for (size_t l = 0u; l < nloci; ++l) {

        // Update numbers of loci for each trait...
        switch (traits[l]) {

        case 0u: ++ntol; break;
        case 1u: ++ncomp; break;
        case 2u: ++nneut; break;

        // ... and check that there are no weird traits
        default: std::runtime_error("Unknown encoded trait"); break;

        }

        // Genomic locations should be positive
        if (locations[l] < 0.0)
            throw std::runtime_error("Locus found with a negative location");

        // Check that the locus is within the specified genomic bounds
        if (locations[l] > chromends.back())
            throw std::runtime_error("Locus found beyond the end of the last chromosome");

        // Check that it is on an existing chromosome
        assert(chromosomes[l] < nchrom);

    }

    // Check that enough loci were provided
    if (ntol < 1u || ncomp < 1u || nneut < 1u)
        throw std::runtime_error("Too few loci provided for at least one trait");

}

size_t Architecture::getNLoci() const { return nloci; }
size_t Architecture::getNTol() const { return ntol; }
size_t Architecture::getNComp() const { return ncomp; }
size_t Architecture::getNNeut() const { return nneut; }
size_t Architecture::getNChrom() const { return nchrom; }
double Architecture::getChromEnd(const size_t &k) const { return chromends[k]; }
double Architecture::getLocation(const size_t &l) const { return locations[l]; }
size_t Architecture::getChromosome(const size_t &l) const { return chromosomes[l]; }
size_t Architecture::getTrait(const size_t &l) const { return traits[l]; }
double Architecture::getEffect(const size_t &l) const { return effects[l]; }
