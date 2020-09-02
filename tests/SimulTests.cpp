#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include "library/Simul.h"
#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_CASE(useSimulate) {

    // Check that the main simulation function works
    BOOST_CHECK_EQUAL(simulate(), 0u);

}
