#include "src/individual.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(knownFitness) {

    Parameters pars = Parameters();
    pars.costtol = 0.0;
    pars.costcomp = 0.0;
    pars.tradeoff = 0.0;
    pars.npatches = 3u;
    Landscape lnd = Landscape(pars);
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

BOOST_AUTO_TEST_CASE(knownTraitValues) {

    Parameters pars = Parameters();
    Architecture arch = Architecture(pars);
    Individual ind = Individual();
    ind.develop(arch);
    BOOST_CHECK_EQUAL(ind.getTolerance(), 0.0);
    BOOST_CHECK_EQUAL(ind.getCompetitiveness(), 1.0);
    BOOST_CHECK_EQUAL(ind.getNeutral(), 0.0);

}

BOOST_AUTO_TEST_CASE(backcrossPredictableIfNoRecombination)
{

    Parameters pars;
    pars.nchrom = 1u; // one chromosome to avoid free recombination
    pars.recombination = 0.0; // no recombination
    Architecture arch = Architecture(pars);
    Individual mom = Individual(); // aa
    Individual dad = Individual(); // AA
    dad.mutateGenome(1.0, arch.getNLoci());
    Individual f1 = Individual(); // Aa
    f1.inherit(mom, pars, arch, false);
    f1.inherit(dad, pars, arch, true);
    Individual backcross = Individual();
    backcross.inherit(mom, pars, arch, false);
    backcross.inherit(f1, pars, arch, true);

    // Backcross should be either full aa or full Aa if no recombination
    const size_t onealleles = backcross.getNOneAlleles();
    BOOST_CHECK(onealleles == 0u || onealleles == arch.getNLoci());

}

BOOST_AUTO_TEST_CASE(backcrossUnpredictableIfRecombination)
{

    Parameters pars;
    pars.recombination = 10.0; // high recombination
    pars.ntol = 10u;
    pars.ncomp = 10u;
    pars.nneut = 10u;
    Architecture arch = Architecture(pars);
    Individual mom = Individual(); // aa
    Individual dad = Individual(); // AA
    dad.mutateGenome(1.0, arch.getNLoci());
    Individual f1 = Individual(); // Aa
    f1.inherit(mom, pars, arch, false);
    f1.inherit(dad, pars, arch, true);
    Individual backcross = Individual();
    backcross.inherit(mom, pars, arch, false);
    backcross.inherit(f1, pars, arch, true);

    // Backcross should not be full aa or full Aa
    const size_t onealleles = backcross.getNOneAlleles();
    BOOST_CHECK(onealleles > 0u && onealleles != arch.getNLoci());

}

