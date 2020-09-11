#ifndef BRACHYPODE_POPULATION_H
#define BRACHYPODE_POPULATION_H

#include "Individual.h"

class Population {

public:

    Population(const size_t&);

private:

    std::vector<Individual> pop;

};

#endif
