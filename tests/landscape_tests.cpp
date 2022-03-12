#include "src/landscape.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(landscapeLoading) {

    // Prepare a file
    std::ofstream file;
    file.open("landscape.txt");
    BOOST_CHECK(file.is_open());

    // Example parameters
    std::vector<double> areas = {10.3, 2.0, 1.0};
    std::vector<size_t> capacities = {50u, 20u, 20u};
    std::vector<size_t> habitats = {0u, 1u, 1u};
    std::vector<double> stresses = {5.0, 0.0, 0.0};
    std::vector<double> competitions = {0.0, 0.1, 0.1};

    // Write them to the file
    file << "npatches " << areas.size() << '\n'
    << "areas " << areas[0u] << '\t' << areas[1u] << '\t' << areas[2u] << '\n'
    << "capacities " << capacities[0u] << '\t' << capacities[1u] << '\t' << capacities[2u] << '\n'
    << "habitats " << habitats[0u] << '\t' << habitats[1u] << '\t' << habitats[2u] << '\n'
    << "stresses " << stresses[0u] << '\t' << stresses[1u] << '\t' << stresses[2u] << '\n'
    << "competitions " << competitions[0u] << '\t' << competitions[1u] << '\t' << competitions[2u] << '\n';

    file.close();

    Parameters pars = Parameters();
    Landscape lnd = Landscape(pars);
    lnd.load();

    // Check that the parameters were properly read
    for (size_t i = 0u; i < lnd.getNPatches(); ++i) {

        BOOST_CHECK_EQUAL(lnd.getArea(i), areas[i]);
        BOOST_CHECK_EQUAL(lnd.getCapacity(i), capacities[i]);
        BOOST_CHECK_EQUAL(lnd.getHabitat(i), habitats[i]);
        BOOST_CHECK_EQUAL(lnd.getStress(i), stresses[i]);
        BOOST_CHECK_EQUAL(lnd.getCompetition(i), competitions[i]);

    }
}
