#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test functionalities of the Individual class.

#include "../src/individual.hpp"
#include <boost/test/unit_test.hpp>

// Test that an individual is initialized properly
BOOST_AUTO_TEST_CASE(individualInitialization) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 3u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Check attributes
    BOOST_CHECK_EQUAL(ind.getDeme(), 0u);
    BOOST_CHECK_EQUAL(ind.getPatch(), 1u);
    BOOST_CHECK_EQUAL(ind.getNSeeds(), 0u);
    BOOST_CHECK_EQUAL(ind.getTolerance(), 0.0);
    BOOST_CHECK(ind.isAlive());
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);

}

// Test that an individual is dead after we kill it
BOOST_AUTO_TEST_CASE(individualIsDeadAfterBeingKilled) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 3u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Kill it
    ind.kill();

    // Should be dead
    BOOST_CHECK(!ind.isAlive());

}

// Test that an individual has its deme changed properly
BOOST_AUTO_TEST_CASE(changeInDeme) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 3u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Update the deme
    ind.setDeme(1u);

    // Check
    BOOST_CHECK_EQUAL(ind.getDeme(), 1u);

}

// Test that an individual has its patch changed properly
BOOST_AUTO_TEST_CASE(changeInPatch) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 3u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Update the patch
    ind.setPatch(1u);

    // Check
    BOOST_CHECK_EQUAL(ind.getPatch(), 1u);

}

// Test that assigning number of seeds works
BOOST_AUTO_TEST_CASE(changeInNSeeds) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 3u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Update the patch
    ind.setNSeeds(666u);

    // Check
    BOOST_CHECK_EQUAL(ind.getNSeeds(), 666u);

}

// Test that an individual has its trait value changed properly
BOOST_AUTO_TEST_CASE(changeInTrait) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 3u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Update trait
    ind.setTolerance(3.0);

    // Check
    BOOST_CHECK_EQUAL(ind.getTolerance(), 3.0);

}

// Test that no mutation does not change the genome
BOOST_AUTO_TEST_CASE(noMutationDoesNotChangeTheGenome) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 20u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Get sum of alleles
    const size_t sum = ind.countAlleles();

    // Mutate with zero mutation rate
    ind.mutate(0.0);

    // Re-compute sum of alleles
    const size_t newsum = ind.countAlleles();

    // Should not have changed
    BOOST_CHECK_EQUAL(sum, newsum);

}

// Test that full mutation changes the whole genome
BOOST_AUTO_TEST_CASE(fullMutationChangesTheWholeGenome) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 20u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Mutate with certainty
    ind.mutate(1.0);

    // All alleles should be one
    BOOST_CHECK_EQUAL(ind.countAlleles(), 20u);

    // Trait should have changed
    BOOST_CHECK_CLOSE(ind.getTolerance(), 2.0, 0.0001);


}

// Test that no change if recombination is zero
BOOST_AUTO_TEST_CASE(noChangeIfRecombinationIsZero) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 5u, 0.1);

    // Note: there cannot be free recombination since only one chromosome

    // Create individual
    Individual ind(0.0, arch);

    // Create a pollen donor individual
    Individual pollen(0.0, arch);

    // Mutate the donor to make sure only 1-alleles
    pollen.mutate(1.0);

    // Recombine with rate zero
    ind.recombine(0.0, pollen);

    // Should not have inherited any of the 1-alleles
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);

}

// Test that recombination produces intermediate phenotypes
BOOST_AUTO_TEST_CASE(nonZeroRecombination) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(1u, 5u, 0.1);

    // Create individual
    Individual ind(0.0, arch);

    // Record allele sum
    const size_t mum = ind.countAlleles();

    // Create a pollen donor individual
    Individual pollen(0.0, arch);

    // Mutate the donor to make sure only 1-alleles
    pollen.mutate(1.0);

    // Record allele sum
    const size_t dad = pollen.countAlleles();

    // Recombine with some rate
    ind.recombine(0.5, pollen);

    // New sum of alleles should be between original and pollen donor
    BOOST_CHECK(ind.countAlleles() >= mum);
    BOOST_CHECK(ind.countAlleles() <= dad);

}