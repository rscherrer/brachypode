#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include "src/simul.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(use) {

    // Check that the main simulation function works
    BOOST_CHECK_EQUAL(simulate({"program_name"}), 0u);

}
