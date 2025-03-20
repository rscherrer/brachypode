#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the printing functions.

#include "testutils.hpp"
#include "../src/printer.hpp"
#include <boost/test/unit_test.hpp>

// Test that updating the requested outputs works
BOOST_AUTO_TEST_CASE(updatingRequestedOutputs) {

    // Set the list of valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Open a file stream
    std::ofstream file("whattosave.txt");

    // Write some requested outputs to the file
    file << "foo\n";
    file << "bar\n";
    file.close();

    // Set a list of requested outputs so far
    std::vector<std::string> outputs = {"time", "popsize", "patchsizes"};

    // Update the requested outputs using the reading function
    stf::read(outputs, "whattosave.txt", valid);

    // Check that the new outputs have changed as expected
    BOOST_CHECK_EQUAL(outputs.size(), 2u);
    BOOST_CHECK_EQUAL(outputs[0u], "foo");
    BOOST_CHECK_EQUAL(outputs[1u], "bar");

}

// Test that updating fails if file not found
BOOST_AUTO_TEST_CASE(updatingFailsWhenFileNotFound) {

    // Set the list of valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Set a list of requested outputs so far
    std::vector<std::string> outputs = {"time", "popsize", "patchsizes"};

    // Check that updating errors when the file is not found
    tst::checkError([&]() {
        stf::read(outputs, "nonexistent.txt", valid);
    }, "Unable to open file nonexistent.txt");

}

// Test that updating fails when invalid output requested
BOOST_AUTO_TEST_CASE(updatingFailsWhenInvalidOutputRequested) {

    // Set the list of valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Open a file stream
    std::ofstream file("whattosave.txt");

    // Write some requested outputs to the file
    file << "foo\n";
    file << "bar\n";
    file << "qux\n";
    file.close();

    // Set a list of requested outputs so far
    std::vector<std::string> outputs = {"time", "popsize", "patchsizes"};
    
    // Check that updating errors when an invalid output is requested
    tst::checkError([&]() {
        stf::read(outputs, "whattosave.txt", valid);
    }, "Invalid output requested in whattosave.txt: qux");

}

// Test that opening and closing buffers works
BOOST_AUTO_TEST_CASE(openingBuffersAndClosingThem) {

    // Set the list of buffer names
    std::vector<std::string> names = {"foo", "bar", "baz"};

    // Set up a map of buffers
    std::unordered_map<std::string, std::optional<Buffer> > buffers;

    // Open the buffers
    stf::open(buffers, names);

    // Check that the buffers have been created
    BOOST_CHECK_EQUAL(buffers.size(), 3u);
    BOOST_CHECK(buffers.find("foo") != buffers.end());
    BOOST_CHECK(buffers.find("bar") != buffers.end());
    BOOST_CHECK(buffers.find("baz") != buffers.end());

    // Check that the buffers are open
    BOOST_CHECK(buffers["foo"]->isopen());
    BOOST_CHECK(buffers["bar"]->isopen());
    BOOST_CHECK(buffers["baz"]->isopen());

    // Close the buffers
    stf::close(buffers);

    // Check that the buffers have been closed
    BOOST_CHECK_EQUAL(buffers.size(), 3u);
    BOOST_CHECK(!buffers["foo"]->isopen());
    BOOST_CHECK(!buffers["bar"]->isopen());
    BOOST_CHECK(!buffers["baz"]->isopen());
    
}