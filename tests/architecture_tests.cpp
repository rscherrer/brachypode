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

    // Create an architecture with 3 chromosomes and 3 loci
    Architecture arch(3u, 3u, 0.1);
    BOOST_CHECK_EQUAL(arch.chromends.size(), 3u); // number of chromosomes
    BOOST_CHECK_EQUAL(arch.locations.size(), 3u); // number of loci
    BOOST_CHECK_EQUAL(arch.effects.size(), 3u); // number of loci

    // For each chromosome...
    for (size_t k = 0u; k < arch.chromends.size(); ++k) {

        // Check the order of chromosome ends
        if (k > 0u) BOOST_CHECK(arch.chromends[k] > arch.chromends[k - 1u]);

        // Check the bounds
        BOOST_CHECK(arch.chromends[k] >= 0.0);
        BOOST_CHECK(arch.chromends[k] <= 1.0);

    }

    // For each locus...
    for (size_t l = 0u; l < arch.locations.size(); ++l) {

        // Check the order of the locations
        if (l > 0u) BOOST_CHECK(arch.locations[l] > arch.locations[l - 1u]);

        // Check the bounds
        BOOST_CHECK(arch.locations[l] >= 0.0);
        BOOST_CHECK(arch.chromends[l] <= arch.chromends.back());

    }

}

// Same for a loaded architecture
BOOST_AUTO_TEST_CASE(loadedArchitectureHasRightAttributes) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

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
    arch.load("architecture.txt");

    // Check no. of chromosomes and loci
    BOOST_CHECK_EQUAL(arch.chromends.size(), 2u);
    BOOST_CHECK_EQUAL(arch.locations.size(), 4u);
    BOOST_CHECK_EQUAL(arch.effects.size(), 4u);

    // Check each chromosome end and each locus position
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

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Check that error when loading non-existing file
    tst::checkError([&] { arch.load("nonexistant.txt"); }, "Unable to open file architecture.txt");

}

// Test that error when cannot read the number of chromosomes
BOOST_AUTO_TEST_CASE(errorWhenCannotReadNumberOfChromosomes) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // The first thing shoulld be the number of chromosomes 
    tst::write("architecture.txt", "hello\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Could not read the number of chromosomes in architecture file");

}

// Test that error if the number of chromosomes is zero
BOOST_AUTO_TEST_CASE(errorWhenZeroChromosomesInArchitecture) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "0\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "There should be at least one chromosome in architecture file");

}

// Test that error if cannot read the end of a chromosome
BOOST_AUTO_TEST_CASE(errorWhenCannotReadChromosomeEnd) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "1 hello\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Could not read the end of chromosome 0 in architecture file");

}

// Test that error when chromosome ends are not in increasing order
BOOST_AUTO_TEST_CASE(errorWhenChromosomeEndsNotInIncreasingOrder) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.8 0.3\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Chromosome ends should be in increasing order in architecture file");

}

// Test that error if chromosomes start before zero
BOOST_AUTO_TEST_CASE(errorWhenChromosomesStartBeforeZero) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 -0.1 0.3 1.0\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Chromosome ends should be positive in architecture file");

}

// Test that error if chromosomes do not end at one
BOOST_AUTO_TEST_CASE(errorWhenChromosomesDoNotEndAtOne) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 0.8\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "End of the last chromosome should be one in architecture file");

    // Write another
    tst::write("architecture.txt", "3 0.1 0.3 1.1\n");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "End of the last chromosome should be one in architecture file");

}

// Test that error if the number of loci cannot be read
BOOST_AUTO_TEST_CASE(errorWhenCannotReadNumberOfLoci) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\nhello");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Could not read the number of loci in architecture file");

}

// Test that error if the number of loci is zero
BOOST_AUTO_TEST_CASE(errorWhenZeroLociInArchitecture) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n0");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "There should be at least one locus in architecture file");

}

// Test that error when locus location cannot be read
BOOST_AUTO_TEST_CASE(errorWhenCannotReadLocusLocation) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 hello");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Could not read the location of locus 2 in architecture file");

}

// Test that error when loci are not in increasing order of location
BOOST_AUTO_TEST_CASE(errorWhenLocusLocationsNotInIncreasingOrder) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.3 0.2");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Locus locations should be in increasing order in architecture file");

}

// Test that error when loci start before zero
BOOST_AUTO_TEST_CASE(errorWhenLociStartBeforeZero) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 -0.1 0.2 0.3");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Locus location should be positive in architecture file");

}

// Test that error when loci go beyond one
BOOST_AUTO_TEST_CASE(errorWhenLociGoBeyondOne) {

    // Create an architecture
    Architecture arch(3u, 3u, 0.1);

    // Write architecture file
    tst::write("architecture.txt", "3 0.1 0.3 1.0\n3 0.1 0.2 1.3");

    // Check error
    tst::checkError([&] { arch.load("architecture.txt"); }, "Locus location should not be beyond the end of the last chromosome in architecture file");

}