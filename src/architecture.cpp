#include "architecture.h"

Architecture::Architecture(const Param &p) :
    nloci(p.ncomp + p.ntol + p.nneut),
    chromends(std::vector<double>(p.nchrom)),
    locations(std::vector<double>(nloci)),
    chromosomes(std::vector<size_t>(nloci)),
    traits(std::vector<size_t>(nloci))//,
    //effects(std::vector<double>(nloci, 0.01))
{

    // Assign end locations to each chromosome
    for (size_t k = 0u; k < chromends.size(); ++k) {
        chromends[k] = (k + 1.0) / chromends.size();
        //std::cout << chromends[k] << '\n';
    }

    // Sample locus relative locations along the genome
    auto getLocation = rnd::uniform(0.0, 1.0);

    for (size_t l = 0u; l < nloci; ++l) {
        locations[l] = getLocation(rnd::rng);
        //std::cout << locations[l] << '\n';
    }

    std::sort(locations.begin(), locations.end());

    //for (size_t l = 0u; l < nloci; ++l) std::cout << locations[l] << '\n';

    // Assign chromosomes to each locus
    for (size_t l = 0u, k = 0u; l < nloci && k < p.nchrom; ) {
        if (locations[l] < chromends[k]) {
            chromosomes[l] = k;
            ++l;
        } else ++k;
    }

    //for (size_t l = 0u; l < nloci; ++l) std::cout << chromosomes[l] << '\n';

    // Randomly assign encoded trait to each locus
    size_t l = 0u;
    for (; l < p.ntol; ++l) traits[l] = 0u;
    for (; l < p.ntol + p.ncomp; ++l) traits[l] = 1u;
    for (; l < nloci; ++l) traits[l] = 2u;

    //for (size_t i = 0u; i < nloci; ++i) std::cout << traits[i] << '\n';

    std::shuffle(traits.begin(), traits.end(), rnd::rng);

    for (size_t i = 0u; i < nloci; ++i) std::cout << traits[i] << '\n';

    assert(chromends.size() == p.nchrom);
    assert(locations.size() == nloci);
    assert(chromosomes.size() == nloci);
    assert(traits.size() == nloci);
    //assert(effects.size() == nloci);

}

size_t Architecture::getNLoci() const { return nloci; }
size_t Architecture::getNChrom() const { return chromends.size(); }
double Architecture::getChromend(const size_t &k) const { return chromends[k]; }
double Architecture::getLocation(const size_t &l) const { return locations[l]; }
size_t Architecture::getChromosome(const size_t &l) const { return chromosomes[l]; }
size_t Architecture::getTrait(const size_t &l) const { return traits[l]; }
//double Architecture::getEffect(const size_t &l) const { return effects[l]; }

