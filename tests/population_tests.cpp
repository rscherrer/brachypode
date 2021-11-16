#include "src/population.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(initPopRightSize) {

    Param pars = Param();
    pars.popsize = 10u;
    Landscape lnd = Landscape(3u);
    Population pop(pars, lnd);

    BOOST_CHECK(pop.getSize() == 10u);

}

BOOST_AUTO_TEST_CASE(fitnessesAreCorrectlyBounded) {

    Param pars = Param();
    Landscape lnd = Landscape(3u);
    Population pop(pars, lnd);

    pop.setFitnesses(pars, lnd);

    for (size_t i = 0u; i < pop.getSize(); ++i) {
        BOOST_CHECK(pop.getFitness(i) <= pars.maxgrowth);
        BOOST_CHECK(pop.getFitness(i) >= 0.0);
    }

}

BOOST_AUTO_TEST_CASE(reproductionIncreasesPopSize) {

    Param pars = Param();
    pars.costtol = 0.0;
    pars.costcomp = 0.0;
    pars.tradeoff = 0.0;
    pars.popsize = 20;
    Landscape lnd = Landscape(3u);
    Population pop(pars, lnd);

    const size_t oldsize = pop.getSize();

    pop.setFitnesses(pars, lnd);
    pop.reproduce(lnd);

    BOOST_CHECK(pop.getSize() > oldsize); // most times
}
