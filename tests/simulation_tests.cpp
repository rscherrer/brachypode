#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include "src/simulation.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(useCase) {

    // Check that the main simulation function works
    BOOST_CHECK_EQUAL(simulate({"program_name"}), 0);

}

/*
BOOST_AUTO_TEST_CASE(extinctionCase) {

    std::ofstream file;
    file.open("parameters.txt");
    if (!file.is_open())
      std::cout << "Unable to open valid parameter test file.\n";
    file << "maxgrowth 0"; // with this the pop should go extinct
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 0);

}
*/

BOOST_AUTO_TEST_CASE(badParameters) {

    std::ofstream file;
    file.open("parameters.txt");
    if (!file.is_open())
      std::cout << "Unable to open valid parameter test file.\n";

    file << "area 0 0\n";
    file << "stress -1 -1\n";
    file << "maxgrowth -1\n";
    file << "steepness -1\n";
    file << "costcomp -1\n";
    file << "costtol -1\n";
    file << "tradeoff -1\n";
    file << "selfing -1\n";
    file << "nchrom 0\n";
    file << "ncomp 0\n";
    file << "ntol 0\n";
    file << "nneut 0\n";
    file << "allfreq -1\n";
    file << "mutation -1\n";
    file << "recombination -1\n";
    file << "effect -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}
