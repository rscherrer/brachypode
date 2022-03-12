#include "src/population.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(initPopRightSize) {

    Parameters pars = Parameters();
    pars.popsize = 10u;
    Landscape lnd = Landscape(pars);
    Architecture arch = Architecture(pars);
    Population pop(pars, lnd, arch);

    BOOST_CHECK(pop.getSize() == 10u);

}

BOOST_AUTO_TEST_CASE(fitnessesAreCorrectlyBounded) {

    Parameters pars = Parameters();
    Landscape lnd = Landscape(pars);
    Architecture arch = Architecture(pars);
    Population pop(pars, lnd, arch);
    pop.assignFitnesses(pars, lnd);

    for (size_t i = 0u; i < pop.getSize(); ++i) {
        BOOST_CHECK(pop.getFitness(i) <= pars.maxgrowth);
        BOOST_CHECK(pop.getFitness(i) >= 0.0);
    }
}

/*
BOOST_AUTO_TEST_CASE(noBabies) {

    Param pars = Param();
    pars.maxgrowth = 0.0;
    Landscape lnd = Landscape(pars);
    Architecture arch = Architecture(pars);
    Population pop = Population(pars, lnd, arch);
    pop.assignFitnesses(pars, lnd);
    pop.lifeCycle(pars, lnd, arch);

    BOOST_CHECK_EQUAL(pop.getSize(), 0u);

}
*/

BOOST_AUTO_TEST_CASE(selfingProducesClones) {

    Parameters pars = Parameters();
    pars.selfing = 1.0;
    pars.popsize = 1u;
    pars.mutation = 0.0;
    Landscape lnd = Landscape(pars);
    Architecture arch = Architecture(pars);
    Population pop = Population(pars, lnd, arch);
    const double tol = pop.getTolerance(0u);
    pop.assignFitnesses(pars, lnd);
    pop.lifeCycle(pars, lnd, arch);

    for (size_t i = 0u; i < pop.getSize(); ++i)
        BOOST_CHECK_EQUAL(tol, pop.getTolerance(i));

}
