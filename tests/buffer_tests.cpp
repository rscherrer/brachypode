#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the data saving buffers.

#include "../src/buffer.hpp"
#include <boost/test/unit_test.hpp>

// Test that the buffer initializes well and can store data
BOOST_AUTO_TEST_CASE(bufferCanStoreData) {

    // Create a fresh buffer
    Buffer buffer(10u);

    // Store a value into it
    buffer.store(3.14);

    // Check it
    BOOST_CHECK_EQUAL(buffer.last(), 3.14);

    // Store another value
    buffer.store(42.0);

    // Check the number of stored values
    BOOST_CHECK_EQUAL(buffer.nstored(), 2u);

    // Check the stored values
    BOOST_CHECK_EQUAL(buffer.last(), 42.0);

}