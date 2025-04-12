#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// These are tests of the good behavior of the Architecture structure. We
// test that the architecture, whether generated or loaded from a file, has
// the expected attributes.

#include "testutils.hpp"
#include "../src/architecture.hpp"
#include <boost/test/unit_test.hpp>

// Check that a generated architecture has the right attributes
BOOST_AUTO_TEST_CASE(architectureHasRightAttributes) {

    // Parameters
    Parameters pars;

    // Specify 3 loci
    pars.nloci = 3u;

    // Create an architecture with 3 loci
    Architecture arch(pars);

    // Check sizes
    BOOST_CHECK_EQUAL(arch.locations.size(), 3u);
    BOOST_CHECK_EQUAL(arch.effects.size(), 3u);

    // Check locus locations
    BOOST_CHECK(arch.locations[0u] >= 0.0);
    BOOST_CHECK(arch.locations[1u] > arch.locations[0u]);
    BOOST_CHECK(arch.locations[2u] >= arch.locations[1u]);
    BOOST_CHECK(arch.locations[2u] <= 1.0);
    
    // Check effect sizes
    BOOST_CHECK_EQUAL(arch.effects[0u], 0.1);
    BOOST_CHECK_EQUAL(arch.effects[1u], 0.1);
    BOOST_CHECK_EQUAL(arch.effects[2u], 0.1);

}

// Same for a loaded architecture
BOOST_AUTO_TEST_CASE(loadedArchitectureHasRightAttributes) {

    // Prepare content of architecture file
    std::ostringstream content;

    // Add lines
    content << "nloci 4\n";                    // no. loci
    content << "locations 0.1 0.2 0.6 0.8\n";  // locations of loci
    content << "effects 0.1 0.1 0.2 0.1\n";    // locus effect sizes

    // Write a new architecture file
    tst::write("architecture.txt", content.str());

    // Overwrite the architecture with the one we just saved
    Architecture arch(Parameters(), "architecture.txt");

    // Check no. of chromosomes and loci
    BOOST_CHECK_EQUAL(arch.locations.size(), 4u);
    BOOST_CHECK_EQUAL(arch.effects.size(), 4u);

    // Check each chromosome end, locus position and effect size
    BOOST_CHECK_EQUAL(arch.locations[0u], 0.1);
    BOOST_CHECK_EQUAL(arch.locations[1u], 0.2);
    BOOST_CHECK_EQUAL(arch.locations[2u], 0.6);
    BOOST_CHECK_EQUAL(arch.locations[3u], 0.8);
    BOOST_CHECK_EQUAL(arch.effects[0u], 0.1);
    BOOST_CHECK_EQUAL(arch.effects[1u], 0.1);
    BOOST_CHECK_EQUAL(arch.effects[2u], 0.2);
    BOOST_CHECK_EQUAL(arch.effects[3u], 0.1);

    // Remove files
    std::remove("architecture.txt");

}

// TODO: Test missing architecture file in main

// Test that error if the parameter is no architecture parameter
BOOST_AUTO_TEST_CASE(errorWhenUnknownParameter) {

    // Write architecture file
    tst::write("architecture.txt", "hello 4");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Invalid parameter: hello in line 1 of file architecture.txt");

    // Remove files
    std::remove("architecture.txt");

}

// Test errors when invalid number of loci
BOOST_AUTO_TEST_CASE(errorWhenInvalidNLoci) {

    // Write architecture file
    tst::write("a1.txt", "nloci 4 4");
    tst::write("a2.txt", "nloci 0");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "a1.txt"); }, "Too many values for parameter nloci in line 1 of file a1.txt");
    tst::checkError([&] { Architecture arch(Parameters(), "a2.txt"); }, "Parameter nloci must be strictly positive in line 1 of file a2.txt");

    // Remove files
    std::remove("a1.txt");
    std::remove("a2.txt");

}

// Test errors when invalid locus locations
BOOST_AUTO_TEST_CASE(errorWhenInvalidLocusLocations) {

    // Write architecture file
    tst::write("a1.txt", "nloci 3\nlocations 0.1 0.2 0.3 0.4");
    tst::write("a2.txt", "nloci 3\nlocations 0.1 0.2");
    tst::write("a3.txt", "nloci 3\nlocations 0.1 0.2 1.1");
    tst::write("a4.txt", "nloci 3\nlocations 0.2 0.1 0.3");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "a1.txt"); }, "Too many values for parameter locations in line 1 of file a1.txt");
    tst::checkError([&] { Architecture arch(Parameters(), "a2.txt"); }, "Too few values for parameter locations in line 1 of file a2.txt");
    tst::checkError([&] { Architecture arch(Parameters(), "a3.txt"); }, "Parameter locations must be between 0 and 1 in line 1 of file a3.txt");
    tst::checkError([&] { Architecture arch(Parameters(), "a4.txt"); }, "Parameter locations must be in strictly increasing order in line 1 of file a4.txt");

    // Remove files
    std::remove("a1.txt");
    std::remove("a2.txt");
    std::remove("a3.txt");
    std::remove("a4.txt");

}

// Test errors when invalid effect sizes
BOOST_AUTO_TEST_CASE(errorWhenInvalidEffectSizes) {

    // Write architecture file
    tst::write("a1.txt", "nloci 3\neffects 0.1 0.2 0.3 0.4");
    tst::write("a2.txt", "nloci 3\neffects 0.1 0.2");
    tst::write("a3.txt", "nloci 3\neffects 0.1 0.0 0.3");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "a1.txt"); }, "Too many values for parameter effects in line 1 of file a1.txt");
    tst::checkError([&] { Architecture arch(Parameters(), "a2.txt"); }, "Too few values for parameter effects in line 1 of file a2.txt");
    tst::checkError([&] { Architecture arch(Parameters(), "a3.txt"); }, "Parameter effects must be strictly positive in line 1 of file a3.txt");
    
    // Remove files
    std::remove("a1.txt");
    std::remove("a2.txt");
    std::remove("a3.txt");

}

// Test when architecture file cannot be saved
BOOST_AUTO_TEST_CASE(errorWhenCannotSave) {

    // Check
    tst::checkError([&] { 

        // Parameters
        Parameters pars;

        // Architecture
        Architecture arch(pars);
        
        // Save
        arch.save("");

    }, "Unable to open file ");

}