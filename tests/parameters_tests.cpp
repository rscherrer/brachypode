#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the Parameters class.

#include "testutils.hpp"
#include "../src/parameters.hpp"
#include <boost/test/unit_test.hpp>

// Test that parameters can be created
BOOST_AUTO_TEST_CASE(parametersCreated) {

    // Without input file should work fine
    Parameters pars;

    // Check a few default values
    BOOST_CHECK_EQUAL(pars.popsize, 10u);
    BOOST_CHECK_EQUAL(pars.pgood[0u], 0.8);
    BOOST_CHECK_EQUAL(pars.nloci, 10u);

}

// Parameters can update by reading from a file
BOOST_AUTO_TEST_CASE(readParameters)
{

    // Write a file with custom parameters
    tst::write("parameters.txt", "popsize 42\nselfing 0.8734565362532\n");

    // Read the parameter file
    Parameters pars("parameters.txt");

    // Check that the parameters have been updated
    BOOST_CHECK_EQUAL(pars.popsize, 42u);
    BOOST_CHECK_EQUAL(pars.selfing, 0.8734565362532);

    // Remove files
    std::remove("parameters.txt");

}

// Test that parameter reading fails when the file does not exist
BOOST_AUTO_TEST_CASE(readParametersFailWhenNoFile)
{

    // Try to...
    tst::checkError([&]() {

        // Read non-existing file
        Parameters pars("nonexistent.txt");

    }, "Unable to open file nonexistent.txt");

}

// Test that parameter reading fails when invalid parameters are provided
BOOST_AUTO_TEST_CASE(readParametersFailWhenInvalidParameters)
{

    // Write a file with invalid parameters
    tst::write("parameters.txt", "popsize 10\nselfing 0.95\ninvalid 42\n");

    // Try to...
    tst::checkError([&]() {

        // Read the file
        Parameters pars("parameters.txt");

    }, "Invalid parameter name: invalid");

    // Remove files
    std::remove("parameters.txt");

}

// Test that parameter reading fails when a value could not be read in
BOOST_AUTO_TEST_CASE(readParametersFailWhenValueCannotBeRead)
{

    // Write a file with invalid value
    tst::write("parameters.txt", "popsize 10\nselfing invalid\n");

    // Try to...
    tst::checkError([&]() {

        // Read the file
        Parameters pars("parameters.txt");

    }, "Could not read value for parameter: selfing");

    // Remove files
    std::remove("parameters.txt");

}

// Test that error when initial population size is zero
BOOST_AUTO_TEST_CASE(checkFailsWhenInitialPopSizeIsZero) {
    
    tst::write("parameters.txt", "popsize 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Initial population size must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that error when zero demes
BOOST_AUTO_TEST_CASE(checkFailsWhenZeroDemes) {
    
    tst::write("parameters.txt", "pgood 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Number of demes must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that the proportion of good patches should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenPropGoodPatchesNotBetweenZeroAndOne) {
    
    tst::write("parameters.txt", "pgood 2 -0.1 1\npgoodEnd 0.1 0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Proportion of good patches must be between zero and one");

    tst::write("parameters.txt", "pgood 2 0.1 1.1\npgoodEnd 0.1 0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Proportion of good patches must be between zero and one");

    // Remove files
    std::remove("parameters.txt");

}

// Test that the proportion of good patches after warming should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenPropGoodPatchesAfterWarmingNotBetweenZeroAndOne) {
    
    tst::write("parameters.txt", "pgood 2 0.1 0.1\npgoodEnd -0.1 1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Proportion of good patches after warming must be between zero and one");

    tst::write("parameters.txt", "pgood 2 0.1 0.1\npgoodEnd 0.1 1.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Proportion of good patches after warming must be between zero and one");

    // Remove files
    std::remove("parameters.txt");

}

// Test that carrying capacity must be positive
BOOST_AUTO_TEST_CASE(checkFailsWhenCarryingCapacityNotPositive) {

    tst::write("parameters.txt", "capacities -1.0 100.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Carrying capacity must be strictly positive");

    tst::write("parameters.txt", "capacities 0.0 100.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Carrying capacity must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that carrying capacity after warming cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenCarryingCapacityAfterWarmingNotPositive) {

    tst::write("parameters.txt", "capacitiesEnd -1.0 100.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Carrying capacity after warming must be strictly positive");

    tst::write("parameters.txt", "capacitiesEnd 0.0 100.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Carrying capacity after warming must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that stress level cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenStressLevelNegative) {
    
    tst::write("parameters.txt", "stress -1.0 0.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Stress level must be positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that stress level after warming cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenStressLevelAfterWarmingNegative) {
    
    tst::write("parameters.txt", "stressEnd -1.0 0.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Stress level after warming must be positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that maximum growth rate cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenMaxGrowthRateNegative) {
    
    tst::write("parameters.txt", "maxgrowth -1.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Maximum growth rate must be positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that steepness of the tolerance function cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenSteepnessNegative) {
    
    tst::write("parameters.txt", "steep -1.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Steepness of the tolerance function must be positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that dispersal rate should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenDispersalRateOutOfBounds) {
    
    tst::write("parameters.txt", "dispersal 1.2");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Dispersal rate must be between zero and one");

    tst::write("parameters.txt", "dispersal -0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Dispersal rate must be between zero and one");

    // Remove files
    std::remove("parameters.txt");

}

// Test that mutation rate should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenMutationRateOutOfBounds) {
    
    tst::write("parameters.txt", "mutation 1.2");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Mutation rate must be between zero and one");

    tst::write("parameters.txt", "mutation -0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Mutation rate must be between zero and one");

    // Remove files
    std::remove("parameters.txt");

}

// Test that there cannot be more than 1000 loci
BOOST_AUTO_TEST_CASE(checkFailsWhenTooManyLoci) {
    
    tst::write("parameters.txt", "nloci 1001");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Number of loci must be 1000 at most");

    // Remove files
    std::remove("parameters.txt");

}

// Test that there cannot be zero loci
BOOST_AUTO_TEST_CASE(checkFailsWhenZeroLoci) {
    
    tst::write("parameters.txt", "nloci 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Number of loci must be stricly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that effect size cannot be negative or zero
BOOST_AUTO_TEST_CASE(checkFailsWhenEffectSizeNegative) {

    tst::write("parameters.txt", "effect 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Effect size of loci must be strictly positive");

    tst::write("parameters.txt", "effect -0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Effect size of loci must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that initial allele frequency should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenAlleleFrequencyOutOfBounds) {
    
    tst::write("parameters.txt", "allfreq 1.2");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Initial allele frequency must be between zero and one");

    tst::write("parameters.txt", "allfreq -0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Initial allele frequency must be between zero and one");

    // Remove files
    std::remove("parameters.txt");

}

// Test that trade-off cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenTradeOffNegative) {
    
    tst::write("parameters.txt", "tradeoff -1.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Trade-off strength must be positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that fails when zero or negative non-linearity parameter
BOOST_AUTO_TEST_CASE(checkFailsWhenWrongNonLinearTradeOff) {
    
    tst::write("parameters.txt", "tradeoff 0.5\nnonlinear 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Non-linearity parameter must be strictly positive");

    tst::write("parameters.txt", "tradeoff 0.5\nnonlinear -0.5");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Non-linearity parameter must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that rate of selfing must be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenSelfingRateOutOfBounds) {
    
    tst::write("parameters.txt", "selfing 1.2");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Rate of selfing must be between zero and one");

    tst::write("parameters.txt", "selfing -0.1");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Rate of selfing must be between zero and one");

    // Remove files
    std::remove("parameters.txt");

}

// Test that recombination rate cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenRecombinationRateNegative) {
    
    tst::write("parameters.txt", "recombination -1.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Recombination rate must be positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that precision threshold cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenPrecisionThresholdNegativeOrZero) {
    
    tst::write("parameters.txt", "precis -1.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Precision threshold must be strictly positive");

    tst::write("parameters.txt", "precis 0.0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Precision threshold must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that buffer must be at least capable of containing one vlue
BOOST_AUTO_TEST_CASE(checkFailsWhenMemoryTooSmall) {

    // One byte should be too small
    tst::write("parameters.txt", "memory 0.000001");

    // Check
    tst::checkError([&]{ 
        Parameters pars("parameters.txt"); 
    }, "Memory use per buffer must be at least " + std::to_string(sizeof(double)) + " bytes");

    // Remove files
    std::remove("parameters.txt");

}

// Test that simulation time cannot be zero
BOOST_AUTO_TEST_CASE(checkFailsWhenSimulationTimeZero) {
    
    tst::write("parameters.txt", "tend 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Simulation time must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that save time interval cannot be zero
BOOST_AUTO_TEST_CASE(checkFailsWhenSaveTimeIntervalZero) {
    
    tst::write("parameters.txt", "tsave 0");
    tst::checkError([&]{ Parameters pars("parameters.txt"); }, "Data saving frequency must be strictly positive");

    // Remove files
    std::remove("parameters.txt");

}

// Test that the parameter saving function works
BOOST_AUTO_TEST_CASE(parameterSavingWorks) {

    // Create parameters
    Parameters pars1;

    // Modify
    pars1.nloci = 42u;

    // Save
    pars1.save("parameters.txt");

    // Overwrite the other
    Parameters pars2("parameters.txt");

    // Make sure the values have been properly replaced
    BOOST_CHECK_EQUAL(pars2.nloci, 42u);

    // Remove files
    std::remove("parameters.txt");

}

// Test when parameter saving does not work
BOOST_AUTO_TEST_CASE(errorWhenSavingParameters) {

    // Check
    tst::checkError([&] {

        // Create parameters
        Parameters pars;

        // Save
        pars.save("");

    }, "Unable to open file ");

}