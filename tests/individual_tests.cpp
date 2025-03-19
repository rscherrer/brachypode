#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test functionalities of the Individual class.

#include "../src/individual.hpp"
#include <boost/test/unit_test.hpp>

// Test that an individual is initialized properly
BOOST_AUTO_TEST_CASE(individualInitialization) {

    Individual ind(0.0, {0.1, 0.1, 0.1});
    BOOST_CHECK_EQUAL(ind.getDeme(), 0u);
    BOOST_CHECK_EQUAL(ind.getPatch(), 1u);
    BOOST_CHECK_EQUAL(ind.getX(), 0.0);
    BOOST_CHECK(ind.isAlive());
    BOOST_CHECK_EQUAL(ind.getAlleleSum(), 0u);

}

// Test that an individual is dead after we kill it
BOOST_AUTO_TEST_CASE(individualIsDeadAfterBeingKilled) {

    Individual ind(0.0, {0.1, 0.1, 0.1});
    ind.kill();
    BOOST_CHECK(!ind.isAlive());

}

// Test that an individual has its deme changed properly
BOOST_AUTO_TEST_CASE(changeInDeme) {

    Individual ind(0.0, {0.1, 0.1, 0.1});
    ind.setDeme(1u);
    BOOST_CHECK_EQUAL(ind.getDeme(), 1u);

}

// Test that an individual has its patch changed properly
BOOST_AUTO_TEST_CASE(changeInPatch) {

    Individual ind(0.0, {0.1, 0.1, 0.1});
    ind.setPatch(1u);
    BOOST_CHECK_EQUAL(ind.getPatch(), 1u);

}

// Test that an individual has its trait value changed properly
BOOST_AUTO_TEST_CASE(changeInTrait) {

    Individual ind(0.0, {0.1, 0.1, 0.1});
    ind.setX(3.0);
    BOOST_CHECK_EQUAL(ind.getX(), 3.0);

}

// Test that no mutation does not change the genome
BOOST_AUTO_TEST_CASE(noMutationDoesNotChangeTheGenome) {

    Individual ind(0.0, std::vector<double>(20u, 0.1));
    const size_t sum = ind.getAlleleSum();
    ind.mutate(0.0, 20u); // mu = 0, nloci = 20
    const size_t newsum = ind.getAlleleSum();
    BOOST_CHECK_EQUAL(sum, newsum); // check that no locus has mutated

}

// Test that full mutation changes the whole genome
BOOST_AUTO_TEST_CASE(fullMutationChangesTheWholeGenome) {

    Individual ind(0.0, std::vector<double>(20u, 0.1));
    ind.mutate(1.0, 20u); // mu = 1, nloci = 20
    const size_t newsum = ind.getAlleleSum();
    BOOST_CHECK_EQUAL(newsum, 20u); // check that 20 loci have been mutated

}

// Test development
BOOST_AUTO_TEST_CASE(development) {

    Individual ind(0.0, std::vector<double>(20u, 0.1));
    ind.develop(std::vector<double>(20u, 0.1)); // effect size
    BOOST_CHECK_EQUAL(ind.getX(), 0.0);
    ind.mutate(1.0, 20u); // mu = 1, nloci = 20
    ind.develop(std::vector<double>(20u, 0.1));
    BOOST_CHECK_EQUAL(round(ind.getX() * 1000.0) / 1000.0, 2.0);

}

// Test that no change if recombination is zero
BOOST_AUTO_TEST_CASE(noChangeIfRecombinationIsZero) {

    Individual ind(0.0, std::vector<double>(5u, 0.1));
    const std::vector<double> chromends = {1.0}; // make sure there is no free recombination between chromosomes
    const std::vector<double> locations = {0.1, 0.19, 0.5, 0.8, 0.9};
    Individual pollen(0.0, std::vector<double>(5u, 0.1));
    pollen.mutate(1.0, locations.size());
    ind.recombine(0.0, pollen, chromends, locations);
    BOOST_CHECK_EQUAL(ind.getAlleleSum(), 0u);

}

// Make sure that founder individuals develop
BOOST_AUTO_TEST_CASE(founderDevelops) {

    // Create an individual with default parameters (initial allele frequency, number of loci, effect size)
    Individual ind(1.0, std::vector<double>(50u, 0.1));
    
    // Check that all traits are nonzero (should be the case if has developed)
    BOOST_CHECK(ind.getX() > 0.0);

}