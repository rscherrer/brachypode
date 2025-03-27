#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the functions of namespace Utility.

#include "testutils.hpp"
#include "../src/utilities.hpp"
#include <boost/test/unit_test.hpp>

// TODO: Test the integer checking function

// Test the integral power function
BOOST_AUTO_TEST_CASE(powerFunction) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::pown(2.0, 2), 4.0);
    BOOST_CHECK_EQUAL(utl::pown(2.0, 4), 16.0);
    BOOST_CHECK_EQUAL(utl::pown(2.0, 1), 2.0);
    BOOST_CHECK_EQUAL(utl::pown(2.0, -1), 0.5);
    BOOST_CHECK_EQUAL(utl::pown(2.0, -2), 0.25);

}

// Test the root function
BOOST_AUTO_TEST_CASE(rootFunction) {

    // Known values
    BOOST_CHECK_EQUAL(utl::rootn(4.0, 2), 2.0);
    BOOST_CHECK_EQUAL(utl::rootn(8.0, 3), 2.0);
    BOOST_CHECK_EQUAL(utl::rootn(27.0, 3), 3.0);
    BOOST_CHECK_EQUAL(utl::rootn(16.0, 4), 2.0);
    BOOST_CHECK_EQUAL(utl::rootn(16.0, 4), 2.0);

}

// Negative roots
BOOST_AUTO_TEST_CASE(negativeRoots) {

    // Known values
    BOOST_CHECK_EQUAL(utl::rootn(4.0, -2), 0.5);
    BOOST_CHECK_EQUAL(utl::rootn(32.0, -5), 0.5);
    BOOST_CHECK_EQUAL(utl::rootn(64.0, -6), 0.5);
    BOOST_CHECK_EQUAL(utl::rootn(100.0, -2), 0.1);

}

// Zeroeth root errors
BOOST_AUTO_TEST_CASE(zeroethRootErrors) {

    // Check
    tst::checkError([&]() { utl::rootn(2.0, 0); }, "Zeroeth root is undefined");
    tst::checkError([&]() { utl::rootn(5.0, 0); }, "Zeroeth root is undefined");

}

// Even root of negative number errors
BOOST_AUTO_TEST_CASE(evenRootOfNegativeNumberErrors) {

    // Check
    tst::checkError([&]() { utl::rootn(-2.0, 2); }, "Even root of negative number is not real");
    tst::checkError([&]() { utl::rootn(-5.0, 3); }, "Even root of negative number is not real");

    // Check that odd root works
    BOOST_CHECK_EQUAL(utl::rootn(-8, 3), -2);
    BOOST_CHECK_EQUAL(utl::rootn(-32, 5), -2);
    BOOST_CHECK_EQUAL(utl::rootn(-1, 5), -1);

}
