#include "Population.h"

#include <iostream>

Population::Population(const size_t &n) :
    pop(std::vector<Individual>(n))
{

}

size_t Population::getSize() const {

    return pop.size();

}

void Population::reproduce(const double &birth) {

    // Create a Poisson distribution of offspring number
    auto getClutchSize = rnd::poisson(birth);

    // Sample numbers of offspring for each adult

    std::vector<size_t> offspringNumbers(pop.size());
    size_t noffspring = 0u;

    for (size_t i = 0u; i < pop.size(); ++i) {

        offspringNumbers[i] = getClutchSize(rnd::rng);
        noffspring += offspringNumbers[i];

    }

    // Update the memory allocated to the population
    pop.reserve(pop.size() + noffspring);

    const size_t nadults = pop.size();

    // Add the corresponding numbers of offspring to the population
    for (size_t i = 0u; i < nadults; ++i) {
        for (size_t j = 0u; j < offspringNumbers[i]; ++j) {

            pop.push_back(Individual());

        }
    }

}
