#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the functions of namespace Utility.

#include "testutils.hpp"
#include "../src/utilities.hpp"
#include <boost/test/unit_test.hpp>

// Test the integral power function
BOOST_AUTO_TEST_CASE(powerFunction) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(2.0, 2), 4.0);
    BOOST_CHECK_EQUAL(utl::power(2.0, 4), 16.0);
    BOOST_CHECK_EQUAL(utl::power(2.0, 1), 2.0);
    BOOST_CHECK_EQUAL(utl::power(2.0, -1), 0.5);
    BOOST_CHECK_EQUAL(utl::power(2.0, -2), 0.25);

}