#include "src/individual.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(knownFitness) {

    Param pars = Param();
    pars.costtol = 0.0;
    pars.costcomp = 0.0;
    pars.tradeoff = 0.0;
    Landscape lnd = Landscape(3u);
    Individual ind = Individual();

    // Should be maxed in protected environment when alone
    lnd.setStress(ind.getPatch(), 0.0);
    ind.setTolerance(5.0);
    ind.setFitness(pars, lnd, ind.getCompetitiveness());
    BOOST_CHECK_EQUAL(ind.getFitness(), pars.maxgrowth); // if tol not too small

    // Should be zero in very stressful environment
    lnd.setStress(ind.getPatch(), 10.0);
    ind.setFitness(pars, lnd, 0.0);
    BOOST_CHECK_EQUAL(ind.getFitness(), 0.0);

    // Check that competition does not apply in the matrix
    lnd.setStress(0u, 0.0);
    ind.setFitness(pars, lnd, ind.getCompetitiveness());
    BOOST_CHECK_EQUAL(ind.getFitness(), pars.maxgrowth);

}

BOOST_AUTO_TEST_CASE(noMutations) {

    Individual ind = Individual();
    ind.mutateGenome(0.0, 10u);
    BOOST_CHECK_EQUAL(ind.getNOneAlleles(), 0u);

}

BOOST_AUTO_TEST_CASE(onlyMutations) {

    Individual ind = Individual();
    ind.mutateGenome(1.0, 10u);
    BOOST_CHECK_EQUAL(ind.getNOneAlleles(), 20u);

}
