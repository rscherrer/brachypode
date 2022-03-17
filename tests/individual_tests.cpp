// Here we test functionalities of the Individual class.

#include "src/individual.h"
#include <boost/test/unit_test.hpp>

// Test that an individual is initialized properly
BOOST_AUTO_TEST_CASE(individualInitialization) {

    Individual ind;
    BOOST_CHECK_EQUAL(ind.getDeme(), 0u);
    BOOST_CHECK_EQUAL(ind.getPatch(), 0u);
    BOOST_CHECK_EQUAL(ind.getX(), 0.0);
    BOOST_CHECK(ind.isAlive());
    BOOST_CHECK_EQUAL(ind.getAlleleSum(), 0u);

}

// Test that an individual is dead after we kill it
BOOST_AUTO_TEST_CASE(individualIsDeadAfterBeingKilled) {

    Individual ind;
    ind.kill();
    BOOST_CHECK(!ind.isAlive());

}

// Test that an individual has its deme changed properly
BOOST_AUTO_TEST_CASE(changeInDeme) {

    Individual ind;
    ind.setDeme(1u);
    BOOST_CHECK_EQUAL(ind.getDeme(), 1u);

}

// Test that an individual has its patch changed properly
BOOST_AUTO_TEST_CASE(changeInPatch) {

    Individual ind;
    ind.setPatch(1u);
    BOOST_CHECK_EQUAL(ind.getPatch(), 1u);

}

// Test that an individual has its trait value changed properly
BOOST_AUTO_TEST_CASE(changeInTrait) {

    Individual ind;
    ind.setX(3.0);
    BOOST_CHECK_EQUAL(ind.getX(), 3.0);

}

// Test that no mutation does not change the genome
BOOST_AUTO_TEST_CASE(noMutationDoesNotChangeTheGenome) {

    Individual ind;
    const size_t sum = ind.getAlleleSum();
    ind.mutate(0.0, 20u); // mu = 0, nloci = 20
    const size_t newsum = ind.getAlleleSum();
    BOOST_CHECK_EQUAL(sum, newsum); // check that no locus has mutated

}

// Test that full mutation changes the whole genome
BOOST_AUTO_TEST_CASE(fullMutationChangesTheWholeGenome) {

    Individual ind;
    ind.mutate(1.0, 20u); // mu = 1, nloci = 20
    const size_t newsum = ind.getAlleleSum();
    BOOST_CHECK_EQUAL(newsum, 20u); // check that 20 loci have been mutated

}

// Test development
BOOST_AUTO_TEST_CASE(development) {

    Individual ind;
    ind.develop(1.0); // effect = 1
    BOOST_CHECK_EQUAL(ind.getZ(), 0.0);
    ind.mutate(1.0, 20u); // mu = 1, nloci = 20
    ind.develop(1.0);
    BOOST_CHECK_EQUAL(ind.getZ(), 20.0);

}

// Test type II development with maximum tolerance
BOOST_AUTO_TEST_CASE(typeIIDevelopmentWithMaximumTolerance) {

    Individual ind;
    ind.mutate(1.0, 20u);
    ind.develop(1.0); // effect = 1
    ind.develop2(20u, 1.0, 1.0, 1.0); // xmax = 1, ymax = 1, tradeoff = 1
    BOOST_CHECK_EQUAL(ind.getZ(), 20.0);
    BOOST_CHECK_EQUAL(ind.getX(), 1.0);

}

// Test type II development with maximum tolerance
BOOST_AUTO_TEST_CASE(typeIIDevelopmentWithMaximumCompetitiveness) {

    Individual ind;
    ind.develop(1.0); // effect = 1
    ind.develop2(20u, 1.0, 1.0, 1.0); // xmax = 1, ymax = 1, tradeoff = 1
    BOOST_CHECK_EQUAL(ind.getZ(), 0.0);
    BOOST_CHECK_EQUAL(ind.getY(), 1.0);

}

// Test that no change if recombination is zero
BOOST_AUTO_TEST_CASE(noChangeIfRecombinationIsZero) {

    Individual ind;
    const std::vector<double> chromends = {1.0}; // make sure there is no free recombination between chromosomes
    const std::vector<double> locations = {0.1, 0.19, 0.5, 0.8, 0.9};
    Individual pollen;
    pollen.mutate(1.0, locations.size());
    ind.recombine(0.0, pollen, chromends, locations);
    BOOST_CHECK_EQUAL(ind.getAlleleSum(), 0u);

}




