#ifndef BRACHYPODE_POPULATION_H
#define BRACHYPODE_POPULATION_H

#include "individual.h"
#include "landscape.h"

class Population {

public:

    Population(const Param&, const Landscape&, const Architecture&);

    void assignFitnesses(const Param&, const Landscape&);
    void lifeCycle(const Landscape&);

    size_t getSize() const;
    double getSumFit() const;
    size_t getPatch(const size_t&) const;
    double getFitness(const size_t&) const;
    double getTolerance(const size_t&) const;
    double getCompetitiveness(const size_t&) const;
    double getNeutral(const size_t&) const;

private:

    std::vector<Individual> pop;

};

#endif
