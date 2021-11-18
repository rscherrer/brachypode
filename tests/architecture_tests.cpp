#include "src/architecture.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(chromosomeEnds) {

    Param pars = Param();
    pars.nchrom = 3u;
    Architecture arch = Architecture(pars);

    for (size_t k = 0u; k < arch.getNChrom(); ++k)
        BOOST_CHECK_EQUAL(arch.getChromEnd(k), (k + 1.0) / 3.0);

}

BOOST_AUTO_TEST_CASE(traitsAreCorrectlyEncoded) {

    Param pars = Param();
    pars.ntol = 10u;
    pars.ncomp = 10u;
    pars.nneut = 10u;
    Architecture arch = Architecture(pars);

    size_t ntol = 0u, ncomp = 0u, nneut = 0u, nunknown = 0u;

    for (size_t l = 0u; l < arch.getNLoci(); ++l) {

        const size_t trait = arch.getTrait(l);

        switch (trait) {
        case 0u: ++ntol; break;
        case 1u: ++ncomp; break;
        case 2u: ++nneut; break;
        default: ++nunknown; break;
        }
    }

    BOOST_CHECK_EQUAL(nunknown, 0u);
    BOOST_CHECK_EQUAL(ntol, pars.ntol);
    BOOST_CHECK_EQUAL(ncomp, pars.ncomp);
    BOOST_CHECK_EQUAL(nneut, pars.nneut);

}
