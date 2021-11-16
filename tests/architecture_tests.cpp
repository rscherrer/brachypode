#include "src/architecture.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(chromosomeEnds) {

    Param pars = Param();
    pars.nchrom = 3u;

    Architecture arch = Architecture(pars);

    //for (size_t k = 0u; k < 3u; ++k)
    //    std::cout << "Hello\n";

        //BOOST_CHECK_EQUAL(arch.getChromend(k), (k + 1.0) / pars.nchrom);

}

BOOST_AUTO_TEST_CASE(lociAreCorrectlyOrdered) {

    Param pars = Param();
    pars.ntol = 10u;
    pars.ncomp = 10u;
    pars.nneut = 10u;
    Architecture arch = Architecture(pars);

    double loc = arch.getLocation(0u);

    for (size_t l = 1u; l < arch.getNLoci(); ++l) {

        BOOST_CHECK(arch.getLocation(l) > loc);
        loc = arch.getLocation(l);

    }

}

BOOST_AUTO_TEST_CASE(rightNumberOfLoci) {

    Param pars = Param();
    pars.ntol = 10u;
    pars.ncomp = 10u;
    pars.nneut = 10u;
    Architecture arch = Architecture(pars);

    bool unknownTrait = false;
    size_t ntol = 0u, ncomp = 0u, nneut = 0;

    for (size_t l = 0u; l < arch.getNLoci(); ++l) {
        const size_t trait = arch.getTrait(l);
        switch (trait) {
        case 0u:
            ++ntol;
            break;
        case 1u:
            ++ncomp;
            break;
        case 2u:
            ++nneut;
            break;
        default:
            unknownTrait = true;
            break;
        }
    }

    BOOST_CHECK(!unknownTrait);
    BOOST_CHECK_EQUAL(ntol, pars.ntol);
    BOOST_CHECK_EQUAL(ncomp, pars.ncomp);
    BOOST_CHECK_EQUAL(nneut, pars.nneut);
    BOOST_CHECK_EQUAL(ntol + ncomp + nneut, arch.getNLoci());

}

