#include "src/architecture.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(chromosomeEnds) {

    Parameters pars = Parameters();
    pars.nchrom = 3u;
    Architecture arch = Architecture(pars);

    for (size_t k = 0u; k < arch.getNChrom(); ++k)
        BOOST_CHECK_EQUAL(arch.getChromEnd(k), (k + 1.0) / 3.0);

}

BOOST_AUTO_TEST_CASE(traitsAreCorrectlyEncoded) {

    Parameters pars = Parameters();
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

/*
BOOST_AUTO_TEST_CASE(architectureLoading) {

    // Prepare a file
    std::ofstream file;
    file.open("architecture.txt");
    BOOST_CHECK(file.is_open());

    // Example parameters
    std::vector<double> chromends = {1.0};
    std::vector<double> locations = {0.1, 0.2, 0.8};
    std::vector<size_t> traits = {0u, 1u, 2u};
    std::vector<double> effects = {0.1, 0.1, 0.2};

    // Write them to the file
    file << "nchrom " << chromends.size() << '\n'
    << "nloci" << locations.size() << '\n'
    << "chromends " << chromends[0u] << '\t' << chromends[1u] << '\t' << chromends[2u] << '\n'
    << "locations " << locations[0u] << '\t' << locations[1u] << '\t' << locations[2u] << '\n'
    << "traits " << traits[0u] << '\t' << traits[1u] << '\t' << traits[2u] << '\n'
    << "effects " << effects[0u] << '\t' << effects[1u] << '\t' << effects[2u] << '\n';

    file.close();

    Param pars = Param();
    Architecture arch = Architecture(pars);
    arch.load();

    // Check that the parameters were correctled loaded
    BOOST_CHECK_EQUAL(arch.getNChrom(), 1u);
    BOOST_CHECK_EQUAL(arch.getNLoci(), 3u);
    BOOST_CHECK_EQUAL(arch.getNTol(), 1u);
    BOOST_CHECK_EQUAL(arch.getNComp(), 1u);
    BOOST_CHECK_EQUAL(arch.getNNeut(), 1u);
    BOOST_CHECK_EQUAL(arch.getChromEnd(0u), 1.0);

    for (size_t l = 0u; l < 3u; ++l) {

        BOOST_CHECK_EQUAL(arch.getLocation(l), locations[l]);
        BOOST_CHECK_EQUAL(arch.getChromosome(l), 0u);
        BOOST_CHECK_EQUAL(arch.getTrait(l), traits[l]);
        BOOST_CHECK_EQUAL(arch.getEffect(l), effects[l]);

    }

}
*/
