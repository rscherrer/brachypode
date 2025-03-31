#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the functions of namespace Utility.

#include "testutils.hpp"
#include "../src/utilities.hpp"
#include <boost/test/unit_test.hpp>

// Test the even checking function
BOOST_AUTO_TEST_CASE(isEven) {

    // Known values
    BOOST_CHECK(!utl::iseven(1));
    BOOST_CHECK(utl::iseven(2));
    BOOST_CHECK(!utl::iseven(3));
    BOOST_CHECK(!utl::iseven(999));
    BOOST_CHECK(utl::iseven(1000));

}

// Test the integer checking function
BOOST_AUTO_TEST_CASE(isInteger) {

    // Known values
    BOOST_CHECK(utl::isinteger(0.0));
    BOOST_CHECK(utl::isinteger(1.0));
    BOOST_CHECK(utl::isinteger(2.0));
    BOOST_CHECK(utl::isinteger(-666.0));
    BOOST_CHECK(!utl::isinteger(0.5));

}

// Test the custom power function when input is zero
BOOST_AUTO_TEST_CASE(powerOfZero) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(0.0, 1.0), 0.0);
    BOOST_CHECK_EQUAL(utl::power(0.0, 6.0), 0.0);
    BOOST_CHECK_EQUAL(utl::power(0.0, -3.0), 0.0);
    BOOST_CHECK_EQUAL(utl::power(0.0, -0.5), 0.0);
    BOOST_CHECK_EQUAL(utl::power(0.0, 0.33), 0.0);

}

// Test the custom power function when input is one
BOOST_AUTO_TEST_CASE(powerOfOne) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(1.0, 1.0), 1.0);
    BOOST_CHECK_EQUAL(utl::power(1.0, 6.0), 1.0);
    BOOST_CHECK_EQUAL(utl::power(1.0, -3.0), 1.0);
    BOOST_CHECK_EQUAL(utl::power(1.0, -0.5), 1.0);
    BOOST_CHECK_EQUAL(utl::power(1.0, 0.33), 1.0);

}

// Test the custom power function when exponent is zero
BOOST_AUTO_TEST_CASE(zeroethPower) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(2.0, 0.0), 1.0);
    BOOST_CHECK_EQUAL(utl::power(3.14, 0.0), 1.0);
    BOOST_CHECK_EQUAL(utl::power(-42, 0.0), 1.0);

}

// Test the custom power function when exponent is one
BOOST_AUTO_TEST_CASE(firstPower) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(2.0, 1.0), 2.0);
    BOOST_CHECK_EQUAL(utl::power(3.14, 1.0), 3.14);
    BOOST_CHECK_EQUAL(utl::power(-42, 1.0), -42.0);

}

// Test the custom power function when exponent is an integer
BOOST_AUTO_TEST_CASE(integerPower) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(2.0, 4.0), 16.0);
    BOOST_CHECK_EQUAL(utl::power(3.0, 3.0), 27.0);
    BOOST_CHECK_EQUAL(utl::power(2.0, -2.0), 0.25);
    BOOST_CHECK_EQUAL(utl::power(-2.0, -4.0), 1.0 / 16.0);

}

// Test the custom power function when exponent is decimal
BOOST_AUTO_TEST_CASE(decimalPower) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::power(4.0, 0.5), 2.0);
    BOOST_CHECK_EQUAL(utl::power(8.0, 1.0 / 3.0), 2.0);
    BOOST_CHECK_EQUAL(utl::power(4.0, -0.5), 0.5);

    // Special case of negative fractional exponents
    BOOST_CHECK(std::isnan(utl::power(-8.0, -1.0 / 3.0)));
    BOOST_CHECK(std::isnan(utl::power(-27.0, -1.0 / 3.0)));

    // Note: Normally those numbers elevated to negative fractionals
    // are real numbers but the complicated underlying power function 
    // uses logarithms to compute the power, which are not defined for
    // negative numbers. It is okay, since in our program we exclusively
    // deal with positive exponents, but keep in mind that this function
    // (just like std::pow()) does not necessary work for all kinds of
    // exponents. 

}

// Test the custom power function when solution is undefined
BOOST_AUTO_TEST_CASE(undefinedPower) {

    // Check some known values
    BOOST_CHECK(std::isnan(utl::power(-4.0, 0.5)));
    BOOST_CHECK(std::isnan(utl::power(-8.0, 2.0 / 5.0)));

}