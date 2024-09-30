#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// These are tests of the good behavior of the Architecture structure. We
// test that the architecture, whether generated or loaded from a file, has
// the expected attributes.

#include "../src/architecture.h"
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

    // Write a new architecture file
    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "2\n"; // no. chromosomes
    archfile << "0.5 1\n"; // ends of chromosomes
    archfile << "4\n"; // no. loci
    archfile << "0.1 0.2 0.6 0.8\n"; // locations of loci
    archfile << "0.1 0.1 0.2 0.1\n"; // locus effect sizes
    archfile.close();

    // Overwrite the architecture with the one we just saved
    arch.load();

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

