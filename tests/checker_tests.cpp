#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the checking functions.

#include "testutils.hpp"
#include "../src/checker.hpp"
#include <boost/test/unit_test.hpp>

// Test the proportion checking function
BOOST_AUTO_TEST_CASE(isProportion) {

    // Known values
    BOOST_CHECK(chk::isproportion(0.0));
    BOOST_CHECK(chk::isproportion(0.5));
    BOOST_CHECK(chk::isproportion(1.0));
    BOOST_CHECK(!chk::isproportion(-0.5));
    BOOST_CHECK(!chk::isproportion(1.5));

    // Strict
    BOOST_CHECK(!chk::isproportion(0.0, true));
    BOOST_CHECK(chk::isproportion(0.5, true));
    BOOST_CHECK(!chk::isproportion(1.0, true));

}

// Test the one-to-thousand checking function
BOOST_AUTO_TEST_CASE(isOneToThousand) {

    // Known values
    BOOST_CHECK(chk::isonetothousand(static_cast<size_t>(1u)));
    BOOST_CHECK(chk::isonetothousand(static_cast<size_t>(1000u)));
    BOOST_CHECK(!chk::isonetothousand(static_cast<size_t>(0u)));
    BOOST_CHECK(!chk::isonetothousand(static_cast<size_t>(1001u)));

    // TODO: Maybe revert that?

}

// Test the memory checking function
BOOST_AUTO_TEST_CASE(isEnoughMB) {

    // Known values
    BOOST_CHECK(chk::isenoughmb(0.1));
    BOOST_CHECK(chk::isenoughmb(1.0));
    BOOST_CHECK(!chk::isenoughmb(1.0 / 8.0));
    BOOST_CHECK(!chk::isenoughmb(1.0 / 16.0));

}

// Test the positive checking function
BOOST_AUTO_TEST_CASE(isPositive) {

    // Known values
    BOOST_CHECK(chk::ispositive(0.0));
    BOOST_CHECK(chk::ispositive(1.0));
    BOOST_CHECK(!chk::ispositive(-1.0));
    BOOST_CHECK(!chk::ispositive(-0.5));

    // Strict
    BOOST_CHECK(!chk::ispositive(0.0, true));
    BOOST_CHECK(chk::ispositive(1.0, true));
    BOOST_CHECK(!chk::ispositive(-1.0, true));

}

// Test the strict positive checking function
BOOST_AUTO_TEST_CASE(isStrictPositive) {

    // Known values
    BOOST_CHECK(!chk::isstrictpos(0.0));
    BOOST_CHECK(chk::isstrictpos(1.0));
    BOOST_CHECK(!chk::isstrictpos(-1.0));
    BOOST_CHECK(!chk::isstrictpos(-0.5));
    BOOST_CHECK(chk::isstrictpos(static_cast<size_t>(1u)));
    BOOST_CHECK(!chk::isstrictpos(static_cast<size_t>(0u)));

}

// Test the order checking function
BOOST_AUTO_TEST_CASE(isInOrder) {

    // Known values
    BOOST_CHECK(chk::isinorder(std::vector<double>{1.0, 2.0, 3.0}));
    BOOST_CHECK(!chk::isinorder(std::vector<double>{1.0, 3.0, 2.0}));
    BOOST_CHECK(!chk::isinorder(std::vector<double>{1.0, 2.0, 1.5}));
    BOOST_CHECK(chk::isinorder(std::vector<double>{1.0, 2.0, 3.0}, true));
    BOOST_CHECK(!chk::isinorder(std::vector<double>{1.0, 1.0, 2.0}, true));

}

// Test the strict order checking function
BOOST_AUTO_TEST_CASE(isStrictOrder) {

    // Known values
    BOOST_CHECK(chk::isstrictorder(std::vector<double>{1.0, 2.0, 3.0}));
    BOOST_CHECK(!chk::isstrictorder(std::vector<double>{1.0, 3.0, 2.0}));
    BOOST_CHECK(!chk::isstrictorder(std::vector<double>{1.0, 2.0, 1.5}));
    BOOST_CHECK(!chk::isstrictorder(std::vector<double>{1.0, 2.0, 2.0}));

}
