#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the data saving buffers.

#include "testutils.hpp"
#include "../src/buffer.hpp"
#include <boost/test/unit_test.hpp>

// Test that the buffer opens properly
BOOST_AUTO_TEST_CASE(bufferOpensProperly) {

    // Create a buffer
    Buffer buffer(10u, "output.dat");

    // Check the size of the buffer
    BOOST_CHECK_EQUAL(buffer.capacity(), 10u);

    // Check that the buffer is empty
    BOOST_CHECK_EQUAL(buffer.size(), 0u);

    // Check that the file has been open
    BOOST_CHECK(buffer.isopen());

}

// Test that buffer errors when cannot open file
BOOST_AUTO_TEST_CASE(bufferCannotOpenFile) {

    // Should error when trying to open a file with empty name
    tst::checkError([&] { Buffer buffer(10u, ""); }, "Unable to open file ");

}

// Test that the buffer stores data properly
BOOST_AUTO_TEST_CASE(bufferStoresProperly) {

    // Create a buffer
    Buffer buffer(10u, "output.dat");

    // Store a value
    buffer.save(3.14);

    // Check that the value was saved
    BOOST_CHECK_EQUAL(buffer.size(), 1u);
    BOOST_CHECK_EQUAL(buffer.last(), 3.14);

}

// Test that the buffer flushes when full
BOOST_AUTO_TEST_CASE(bufferFlushesWhenFull) {

    // Create a buffer
    Buffer buffer(3u, "output.dat");

    // Store more data than the capacity permits
    buffer.save(0.1);
    buffer.save(0.2);
    buffer.save(0.3);
    buffer.save(0.4);

    // Check that the first values have been flushed
    BOOST_CHECK_EQUAL(buffer.size(), 1u);
    BOOST_CHECK_EQUAL(buffer.last(), 0.4);

    // Close the file
    buffer.close();

    // Read the data back in
    std::vector<double> values = tst::read("output.dat");

    // Check them
    BOOST_CHECK_EQUAL(values.size(), 4u);
    BOOST_CHECK_EQUAL(values[0u], 0.1);
    BOOST_CHECK_EQUAL(values[1u], 0.2);
    BOOST_CHECK_EQUAL(values[2u], 0.3);
    BOOST_CHECK_EQUAL(values[3u], 0.4);

}

// Test that the buffer closes properly
BOOST_AUTO_TEST_CASE(bufferClosesProperly) {

    // Create a buffer
    Buffer buffer(10u, "output.dat");

    // Save a value under the capacity
    buffer.save(3.14);

    // Close the buffer
    buffer.close();

    // Check that the file is closed
    BOOST_CHECK(!buffer.isopen());

    // Read the value back in
    std::vector<double> values = tst::read("output.dat");

    // Make sure the value was flushed
    BOOST_CHECK_EQUAL(values.size(), 1u);
    BOOST_CHECK_EQUAL(values[0u], 3.14);

}

// Special case of an empty file
BOOST_AUTO_TEST_CASE(bufferClosesWithEmptyFile) {

    // Create a buffer
    Buffer buffer(10u, "output.dat");

    // Close it
    buffer.close();

    // Read the data back in
    std::vector<double> values = tst::read("output.dat");

    // Make sure it is empty
    BOOST_CHECK_EQUAL(values.size(), 0u);

}