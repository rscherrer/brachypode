#ifndef BRACHYPODE_POPULATION_H
#define BRACHYPODE_POPULATION_H

#include "individual.h"
#include "landscape.h"

class Population {

public:

    Population(const Parameters&, const Landscape&, const Architecture&);

    void assignFitnesses(const Parameters&, const Landscape&);
    void lifeCycle(const Parameters&, const Landscape&, const Architecture&);

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
