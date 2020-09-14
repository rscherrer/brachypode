#include "Simul.h"

// Main simulation function
int simulate() {

    // Create a population
    Population pop = Population(10u);

    assert(pop.getSize() == 10u);

    // Reproduction
    pop.reproduce(2.0);

    return 0;

}
