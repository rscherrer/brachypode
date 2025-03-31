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

    // Specify 4 chromosomes and 3 loci
    pars.nloci = 3u;
    pars.nchrom = 4u;

    // Create an architecture with 4 chromosomes and 3 loci
    Architecture arch(pars);

    // Check sizes
    BOOST_CHECK_EQUAL(arch.chromends.size(), 4u);
    BOOST_CHECK_EQUAL(arch.locations.size(), 3u);
    BOOST_CHECK_EQUAL(arch.effects.size(), 3u);

    // Check chromosome ends
    BOOST_CHECK_EQUAL(arch.chromends[0u], 0.25);
    BOOST_CHECK_EQUAL(arch.chromends[1u], 0.5);
    BOOST_CHECK_EQUAL(arch.chromends[2u], 0.75);
    BOOST_CHECK_EQUAL(arch.chromends[3u], 1.0);

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
    content << "2\n";                // no. chromosomes
    content << "0.5 1\n";            // ends of chromosomes
    content << "4\n";                // no. loci
    content << "0.1 0.2 0.6 0.8\n";  // locations of loci
    content << "0.1 0.1 0.2 0.1\n";  // locus effect sizes

    // Write a new architecture file
    tst::write("architecture.txt", content.str());

    // Overwrite the architecture with the one we just saved
    Architecture arch(Parameters(), "architecture.txt");

    // Check no. of chromosomes and loci
    BOOST_CHECK_EQUAL(arch.chromends.size(), 2u);
    BOOST_CHECK_EQUAL(arch.locations.size(), 4u);
    BOOST_CHECK_EQUAL(arch.effects.size(), 4u);

    // Check each chromosome end, locus position and effect size
    BOOST_CHECK_EQUAL(arch.chromends[0u], 0.5);
    BOOST_CHECK_EQUAL(arch.chromends[1u], 1.0);
    BOOST_CHECK_EQUAL(arch.locations[0u], 0.1);
    BOOST_CHECK_EQUAL(arch.locations[1u], 0.2);
    BOOST_CHECK_EQUAL(arch.locations[2u], 0.6);
    BOOST_CHECK_EQUAL(arch.locations[3u], 0.8);
    BOOST_CHECK_EQUAL(arch.effects[0u], 0.1);
    BOOST_CHECK_EQUAL(arch.effects[1u], 0.1);
    BOOST_CHECK_EQUAL(arch.effects[2u], 0.2);
    BOOST_CHECK_EQUAL(arch.effects[3u], 0.1);

}

// Test that error when cannot read the file
BOOST_AUTO_TEST_CASE(errorWhenCannotReadArchitectureFile) {

    // Check that error when loading non-existing file
    tst::checkError([&] { Architecture arch(Parameters(), "nonexistent.txt"); }, "Unable to open file nonexistent.txt");

}

// Test that error when cannot read the number of chromosomes
BOOST_AUTO_TEST_CASE(errorWhenCannotReadNumberOfChromosomes) {

    // The first thing shoulld be the number of chromosomes 
    tst::write("architecture.txt", "hello\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the number of chromosomes in architecture file");

}

// Test that error if the number of chromosomes is zero
BOOST_AUTO_TEST_CASE(errorWhenZeroChromosomesInArchitecture) {

    // Write architecture file
    tst::write("architecture.txt", "0\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "There must be at least one chromosome in architecture file");

}

// Test that error if cannot read the end of a chromosome
BOOST_AUTO_TEST_CASE(errorWhenCannotReadChromosomeEnd) {

    // Write architecture file
    tst::write("architecture.txt", "1 hello\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the end of chromosome 1 in architecture file");

}

// Test that error when chromosome ends are not in increasing order
BOOST_AUTO_TEST_CASE(errorWhenChromosomeEndsNotInIncreasingOrder) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.8 0.3\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Chromosome ends must be in strictly increasing order in architecture file");

}

// Test that error if chromosomes start before zero
BOOST_AUTO_TEST_CASE(errorWhenChromosomesStartBeforeZero) {

    // Write architecture file
    tst::write("architecture.txt", "3 -0.1 0.3 1.0\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Chromosome ends must be strictly positive in architecture file");

}

// Test that error if chromosomes do not end at one
BOOST_AUTO_TEST_CASE(errorWhenChromosomesDoNotEndAtOne) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 0.8\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "End of the last chromosome must be one in architecture file");

    // Write another
    tst::write("architecture.txt", "3 0.1 0.3 1.1\n");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "End of the last chromosome must be one in architecture file");

}

// Test that error if the number of loci cannot be read
BOOST_AUTO_TEST_CASE(errorWhenCannotReadNumberOfLoci) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\nhello");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the number of loci in architecture file");

}

// Test that error if the number of loci is zero
BOOST_AUTO_TEST_CASE(errorWhenZeroLociInArchitecture) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n0");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "There must be at least one locus in architecture file");

}

// Test that error when locus location cannot be read
BOOST_AUTO_TEST_CASE(errorWhenCannotReadLocusLocation) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 hello");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the location of locus 3 in architecture file");

}

// Test that error when loci are not in increasing order of location
BOOST_AUTO_TEST_CASE(errorWhenLocusLocationsNotInIncreasingOrder) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.3 0.2");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Locus locations must be in strictly increasing order in architecture file");

}

// Test that error when loci start before zero
BOOST_AUTO_TEST_CASE(errorWhenLociStartBeforeZero) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 -0.1 0.2 0.3");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Locus location must be positive in architecture file");

}

// Test that error when loci go beyond one
BOOST_AUTO_TEST_CASE(errorWhenLociGoBeyondOne) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 1.3");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Locus location cannot be beyond the end of the last chromosome in architecture file");

}

// Test that error when cannot read locus effect size
BOOST_AUTO_TEST_CASE(errorWhenCannotReadEffectSize) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 0.3\n0.1 0.1 hello");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Could not read the effect size of locus 3 in architecture file");

}

// Test that error when negative effect size
BOOST_AUTO_TEST_CASE(errorWhenEffectSizeNegative) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 0.3\n0.1 0.1 -0.1");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Effect size of locus 3 must be positive in architecture file");

}

// Test that error when total effect size is zero
BOOST_AUTO_TEST_CASE(errorWhenTotalEffectSizeIsZero) {

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 0.3\n0.0 0.0 0.0");

    // Check error
    tst::checkError([&] { Architecture arch(Parameters(), "architecture.txt"); }, "Sum of effect sizes must be strictly positive in architecture file");

}

