#ifndef BRACHYPODE_POPULATION_H
#define BRACHYPODE_POPULATION_H

#include "Individual.h"

class Population {

public:

    Population(const size_t&);

    size_t getSize() const;

    void reproduce(const double&);

private:

    std::vector<Individual> pop;

};

#endif
