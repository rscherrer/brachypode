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
    content << "4\n";                // no. loci
    content << "0.1 0.2 0.6 0.8\n";  // locations of loci
    content << "0.1 0.1 0.2 0.1\n";  // locus effect sizes

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

// Test that error when cannot read the file
BOOST_AUTO_TEST_CASE(errorWhenCannotReadArchitectureFile) {

    // Check that error when loading non-existing file
    tst::checkError([&] { Architecture arch(Parameters(), "nonexistent.txt"); }, "Unable to open file nonexistent.txt");

}

// Test that error if the number of loci cannot be read
BOOST_AUTO_TEST_CASE(errorWhenCannotReadNumberOfLoci) {

    // Write architecture file
    tst::write("architecture.txt", "hello");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the number of loci in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error if the number of loci is zero
BOOST_AUTO_TEST_CASE(errorWhenZeroLociInArchitecture) {

    // Write architecture file
    tst::write("architecture.txt", "\n0");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "There must be at least one locus in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when locus location cannot be read
BOOST_AUTO_TEST_CASE(errorWhenCannotReadLocusLocation) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.2 hello");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the location of locus 3 in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when loci are not in increasing order of location
BOOST_AUTO_TEST_CASE(errorWhenLocusLocationsNotInIncreasingOrder) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 0.2");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Locus locations must be in strictly increasing order in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when loci start before zero
BOOST_AUTO_TEST_CASE(errorWhenLociStartBeforeZero) {

    // Write architecture file
    tst::write("architecture.txt", "3 -0.1 0.2 0.3");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Locus location must be positive in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when loci go beyond one
BOOST_AUTO_TEST_CASE(errorWhenLociGoBeyondOne) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.2 1.3");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Locus location cannot be beyond the end of the genome");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when cannot read locus effect size
BOOST_AUTO_TEST_CASE(errorWhenCannotReadEffectSize) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.2 0.3\n0.1 0.1 hello");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the effect size of locus 3 in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when negative effect size
BOOST_AUTO_TEST_CASE(errorWhenEffectSizeNegative) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.2 0.3\n0.1 0.1 -0.1");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Effect size of locus 3 must be positive in architecture file");

    // Remove files
    std::remove("architecture.txt");

}

// Test that error when total effect size is zero
BOOST_AUTO_TEST_CASE(errorWhenTotalEffectSizeIsZero) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.2 0.3\n0.0 0.0 0.0");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Sum of effect sizes must be strictly positive in architecture file");

    // Remove files
    std::remove("architecture.txt");

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