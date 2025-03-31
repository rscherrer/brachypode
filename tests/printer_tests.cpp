#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the printing functions.

#include "testutils.hpp"
#include "../src/printer.hpp"
#include <boost/test/unit_test.hpp>

// Test that printer opens buffers properly
BOOST_AUTO_TEST_CASE(printerOpensBuffers) {

    // Valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Create a printer
    Printer print(valid);
    
    // Open buffers
    print.open();

    // Check that the printer is on
    BOOST_CHECK(print.ison());

    // Check that the buffers are open
    BOOST_CHECK(print.check("foo"));
    BOOST_CHECK(print.check("bar"));
    BOOST_CHECK(print.check("baz"));

    // Close
    print.close();

    // Check that the buffers are closed
    BOOST_CHECK(!print.check("foo"));
    BOOST_CHECK(!print.check("bar"));
    BOOST_CHECK(!print.check("baz"));

    // Remove files
    std::remove("foo.dat");
    std::remove("bar.dat");
    std::remove("baz.dat");

} 

// Test that the printer can read requested output
BOOST_AUTO_TEST_CASE(printerReadsRequestedOutput) {

    // Valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Create a printer
    Printer print(valid);

    // Write requested outputs
    tst::write("whattosave.txt", "foo\nbar");

    // Read requested outputs
    print.read("whattosave.txt");

    // Open the buffers
    print.open();

    // Check that the right buffers are open
    BOOST_CHECK(print.check("foo"));
    BOOST_CHECK(print.check("bar"));
    BOOST_CHECK(!print.check("baz"));

    // Remove files
    std::remove("whattosave.txt");
    std::remove("foo.dat");
    std::remove("bar.dat");

}

// Test that updating fails if file not found
BOOST_AUTO_TEST_CASE(printerFailsWhenFileNotFound) {

    // Set the list of valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Create a printer
    Printer print(valid);

    // Check that updating errors when the file is not found
    tst::checkError([&]() {
        print.read("nonexistent.txt");
    }, "Unable to open file nonexistent.txt");

}

// Test that updating fails when invalid output requested
BOOST_AUTO_TEST_CASE(printerFailsWhenInvalidOutputRequested) {

    // Set the list of valid outputs
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Create a printer
    Printer print(valid);

    // Write some requested outputs to file
    tst::write("whattosave.txt", "foo\nbar\nqux");
    
    // Check that updating errors when an invalid output is requested
    tst::checkError([&]() {
        print.read("whattosave.txt");
    }, "Invalid output requested in whattosave.txt: qux");

    // Remove files
    std::remove("whattosave.txt");
    std::remove("foo.dat");
    std::remove("bar.dat");

}

// Test that saving into buffers works
BOOST_AUTO_TEST_CASE(savingIntoBuffers) {

    // Set the list of buffer names
    std::vector<std::string> valid = {"foo", "bar", "baz"};

    // Create a printer
    Printer print(valid);

    // Open
    print.open();

    // Save a double
    print.save("foo", 3.14);

    // An unsigned integer
    print.save("foo", static_cast<size_t>(42u));

    // Close
    print.close();

    // Read the data back in
    std::vector<double> values = tst::read("foo.dat");

    // Check the saved value
    BOOST_CHECK_EQUAL(values.size(), 2u);
    BOOST_CHECK_EQUAL(values[0u], 3.14);
    BOOST_CHECK_EQUAL(values[1u], 42.0);

    // Remove files
    std::remove("foo.dat");
    std::remove("bar.dat");
    std::remove("baz.dat");

}