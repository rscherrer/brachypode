#ifndef BRACHYPODE_POPULATION_H
#define BRACHYPODE_POPULATION_H

#include "individual.h"
#include "landscape.h"

class Population {

public:

    Population(const Param&, const Landscape&);

    void setFitnesses(const Param&, const Landscape&);
    void reproduce(const Landscape&);

    size_t getSize() const;
    size_t getPatch(const size_t&) const;
    double getFitness(const size_t&) const;

private:

    std::vector<Individual> pop;

};

#endif
