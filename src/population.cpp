#include "population.h"

#include <iostream>

Population::Population(const Param &p, const Landscape &l) :
    pop(std::vector<Individual>(p.popsize))
{

    // Distribution of patches weighted by area
    auto pickPatch = rnd::discrete(l.areas.begin(), l.areas.end());

    const size_t nloci = p.ntol + p.ncomp + p.nneut;

    // Sow the founder individuals across the patches and alleles across individuals
    for (size_t i = 0u; i < pop.size(); ++i) {

        pop[i].setPatch(pickPatch(rnd::rng));
        pop[i].mutateGenome(p.allfreq, nloci);

    }
}

void Population::setFitnesses(const Param &p, const Landscape &l) {

    // Compute the total competition intensity in each patch...
    std::vector<double> densities(l.getNPatches(), 0.0);

    for (size_t i = 0u; i < pop.size(); ++i) {

        // ... by summing competitive abilities
        const size_t patch = pop[i].getPatch();
        densities[patch] += pop[i].getCompetitiveness();

    }

    // Then assign fitness to each individual...
    for (size_t i = 0u; i < pop.size(); ++i) {

        // ... depending on its perceived competition from others
        const size_t patch = pop[i].getPatch();
        const double density = densities[patch];

        pop[i].setFitness(p, l, density);

    }
}

// Lambda for removing dead individuals
auto burry = [](Individual ind) -> bool
{
    return !ind.isAlive();
};

void Population::reproduce(const Landscape &l) {

    // Figure how many seeds will be produced
    double sumfit = 0.0;
    std::vector<double> fitnesses(pop.size());

    for (size_t i = 0u; i < pop.size(); ++i) {
        fitnesses[i] = pop[i].getFitness();
        sumfit += fitnesses[i];
    }

    auto getPoolSize = rnd::poisson(sumfit);
    size_t nseeds = getPoolSize(rnd::rng);

    // Note: this is approx. the same as sample family sizes for each plant

    // Figure how many will actually germinate
    size_t capacity = 0u;

    for (size_t j = 0u; j < l.getNPatches(); ++j)
        capacity += l.getCapacity(j);

    if (nseeds > capacity) nseeds = capacity;

    // Sample offspring from parents wrt fitness and disperse them
    auto pickParent = rnd::discrete(fitnesses.begin(), fitnesses.end());
    auto pickPatch = rnd::discrete(l.areas.begin(), l.areas.end());

    pop.reserve(pop.size() + nseeds);

    for (size_t k = 0u; k < nseeds; ++k) {

        const size_t parent = pickParent(rnd::rng);
        const size_t patch = pickPatch(rnd::rng);

        pop.push_back(pop[parent]);
        pop.back().setPatch(patch);

    }

    // Kill all parents
    for (size_t i = 0u; i < pop.size() - nseeds; ++i) pop[i].kill();
    auto it = std::remove_if(pop.begin(), pop.end(), burry);
    pop.erase(it, pop.end());
    pop.shrink_to_fit();

    assert(pop.size() == nseeds);

}

size_t Population::getSize() const { return pop.size(); }
size_t Population::getPatch(const size_t &i) const { return pop[i].getPatch(); }
double Population::getFitness(const size_t &i) const { return pop[i].getFitness(); }
