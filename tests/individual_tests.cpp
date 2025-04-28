#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test functionalities of the Individual class.

#include "../src/individual.hpp"
#include <boost/test/unit_test.hpp>

// Test that an individual is initialized properly
BOOST_AUTO_TEST_CASE(individualInitialization) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(Parameters()));

    // Create individual
    Individual ind(0.0, arch);

    // Check attributes
    BOOST_CHECK_EQUAL(ind.getDeme(), 0u);
    BOOST_CHECK_EQUAL(ind.getPatch(), 1u);
    BOOST_CHECK_EQUAL(ind.getNSeeds(), 0u);
    BOOST_CHECK_EQUAL(ind.getTolerance(), 0.0);
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);

}

// Test that an individual is initialized properly with maximum allele frequency
BOOST_AUTO_TEST_CASE(individualInitializationWithAllMutations) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.effect = 0.1;
    pars.nloci = 20u;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(1.0, arch);

    // Check attributes
    BOOST_CHECK_EQUAL(ind.countAlleles(), 20u);
    BOOST_CHECK_CLOSE(ind.getTolerance(), 2.0, 1E6);

}

// Test that an individual has its deme changed properly
BOOST_AUTO_TEST_CASE(changeInDeme) {

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(Parameters()));

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
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(Parameters()));

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
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(Parameters()));

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
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(Parameters()));

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
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(Parameters()));

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

    // Parameters
    Parameters pars;

    // Tweak
    pars.nloci = 20u;
    pars.effect = 0.1;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(0.0, arch);

    // Mutate with certainty
    ind.mutate(1.0);

    // All alleles should be one
    BOOST_CHECK_EQUAL(ind.countAlleles(), 20u);

    // Trait should have changed
    BOOST_CHECK_CLOSE(ind.getTolerance(), 2.0, 0.0001);


}

// Test the different kinds of mutation
BOOST_AUTO_TEST_CASE(differentMutationTypes) {

    // Parameters
    Parameters pars;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(0.0, arch);

    // Mutate with some rate
    BOOST_CHECK_NO_THROW(ind.mutate(0.9));
    BOOST_CHECK_NO_THROW(ind.mutate(0.6));
    BOOST_CHECK_NO_THROW(ind.mutate(0.05));
    BOOST_CHECK_NO_THROW(ind.mutate(0.001));

}

// Test early exit in binomial mutation
BOOST_AUTO_TEST_CASE(earlyExitInBinomialMutation) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.nloci = 10u;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(0.0, arch);

    // Binomial mutation with full probability
    ind.mutateBinomial(1.0);

    // Check that all alleles have been mutated
    BOOST_CHECK_EQUAL(ind.countAlleles(), 10u); 

    // Revert all mutations with shuffle
    ind.mutateShuffle(1.0);

    // Check
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);
    
    // Run with zero mutations
    ind.mutateBinomial(0.0);

    // Check
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);

    // Same with shuffle
    ind.mutateShuffle(0.0);

    // Check
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);

}

// Cover binomial mutations with nonzero rate (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(nonZeroBinomialMutation) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.nloci = 1000u;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(0.0, arch);

    // Binomial mutation with some probability
    ind.mutateBinomial(0.5);

    // Check that some alleles have been mutated
    BOOST_CHECK(ind.countAlleles() > 0u); 

}

// Test that no change if recombination is zero
BOOST_AUTO_TEST_CASE(noChangeIfRecombinationIsZero) {

    // Parameters
    Parameters pars;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(0.0, arch);

    // Create a pollen donor individual
    Individual pollen(1.0, arch);

    // Recombine with rate zero
    ind.recombine(0.0, pollen);

    // Should not have inherited any of the 1-alleles
    BOOST_CHECK_EQUAL(ind.countAlleles(), 0u);

}

// Test that recombination produces intermediate phenotypes (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(nonZeroRecombination) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.nloci = 100u;

    // Create architecture
    std::shared_ptr<Architecture> arch = std::make_shared<Architecture>(Architecture(pars));

    // Create individual
    Individual ind(0.0, arch);

    // Record allele sum
    const size_t mom = ind.countAlleles();

    // Create a pollen donor individual
    Individual pollen(1.0, arch);

    // Record allele sum
    const size_t dad = pollen.countAlleles();

    // Recombine with some rate
    ind.recombine(30.0, pollen);

    // New sum of alleles should be between original and pollen donor
    BOOST_CHECK(ind.countAlleles() > mom);
    BOOST_CHECK(ind.countAlleles() < dad);

}