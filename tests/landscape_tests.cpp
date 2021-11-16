#include "src/landscape.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(landscapeLoading) {

    // Prepare a landscape file
    std::ofstream file;
    file.open("landscape.txt");
    if (!file.is_open())
      std::cout << "Unable to open valid parameter test file.\n";

    // Specify parameters
    std::vector<double> areas = {10.3, 2.0, 1.0};
    std::vector<size_t> capacities = {50u, 20u, 20u};
    std::vector<size_t> habitats = {0u, 1u, 1u};
    std::vector<double> stresses = {5.0, 0.0, 0.0};
    std::vector<double> competitions = {0.0, 0.1, 0.1};

    // Write them to the file
    file << areas.size() << '\n'
    << areas[0u] << '\t' << areas[1u] << '\t' << areas[2u] << '\n'
    << capacities[0u] << '\t' << capacities[1u] << '\t' << capacities[2u] << '\n'
    << habitats[0u] << '\t' << habitats[1u] << '\t' << habitats[2u] << '\n'
    << stresses[0u] << '\t' << stresses[1u] << '\t' << stresses[2u] << '\n'
    << competitions[0u] << '\t' << competitions[1u] << '\t' << competitions[2u] << '\n';

    file.close();

    // Load the landscape file
    Landscape lnd = Landscape(3u);
    lnd.load();

    // Check that the parameters were properly read
    bool check = true;

    for (size_t i = 0u; i < lnd.getNPatches(); ++i) {

        const bool check1 = lnd.getArea(i) == areas[i];
        const bool check2 = lnd.getCapacity(i) == capacities[i];
        const bool check3 = lnd.getHabitat(i) == habitats[i];
        const bool check4 = lnd.getStress(i) == stresses[i];
        const bool check5 = lnd.getCompetition(i) == competitions[i];

        if (!(check1 && check2 && check3 && check4 && check5)) { check = false; break; }
    }

    BOOST_CHECK(check);

}
