#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the data saving buffers.

#include "testutils.hpp"
#include "../src/buffer.hpp"
#include <boost/test/unit_test.hpp>

// Test that the buffer initializes well and can store data
BOOST_AUTO_TEST_CASE(bufferCanStoreData) {

    // Create a fresh buffer
    Buffer buffer(10u, "values.dat");

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

// Test that the buffer can write to file
BOOST_AUTO_TEST_CASE(bufferCanWriteToFile) {

    // Create a buffer
    Buffer buffer(10u, "values.dat");

    // Store some values
    buffer.store(3.14);
    buffer.store(42.0);

    // Flush the buffer to file
    buffer.flush();

    // Check that the head buffer is now empty (and ready to store new data)
    BOOST_CHECK_EQUAL(buffer.nstored(), 0u);

    // Close the output stream to the file
    buffer.close();

    // Read the saved file
    std::vector<double> values = tst::readBinary("values.dat");

    // Check the saved value
    BOOST_CHECK_EQUAL(values.size(), 2u);
    BOOST_CHECK_EQUAL(values[0u], 3.14);
    BOOST_CHECK_EQUAL(values[1u], 42.0);

}

// Test that the buffer writes to file when its limit storage is reached
BOOST_AUTO_TEST_CASE(bufferFlushesAutomatically) {

    // Create a buffer with a limit of two
    Buffer buffer(2u, "values.dat");

    // Store a value
    buffer.store(3.14);

    // Store another one
    buffer.store(42.0);

    // Check that the buffer now only contains one value
    BOOST_CHECK_EQUAL(buffer.nstored(), 1u);

    // Check the value
    BOOST_CHECK_EQUAL(buffer.last(), 42.0);

    // Close the file
    buffer.close();

    // Read the saved file
    std::vector<double> values = tst::readBinary("values.dat");

    // Check the saved value
    BOOST_CHECK_EQUAL(values.size(), 1u);
    BOOST_CHECK_EQUAL(values[0u], 3.14);

}
