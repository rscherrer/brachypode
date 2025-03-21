#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the Parameters class.

#include "testutils.hpp"
#include "../src/parameters.hpp"
#include <boost/test/unit_test.hpp>

// Parameters can update by reading from a file
BOOST_AUTO_TEST_CASE(readParameters)
{

    // Create parameters
    Parameters pars;

    // Set some values to make sure they are updated
    pars.popsize = 42u;
    pars.selfing = 0.8748673626732362536253;
    
    // Write a file with custom parameters
    tst::write("parameters.txt", "popsize 10\nselfing 0.95\n");

    // Read the parameter file
    pars.read("parameters.txt");

    // Check that the parameters have been updated
    BOOST_CHECK_EQUAL(pars.popsize, 10u);
    BOOST_CHECK_EQUAL(pars.selfing, 0.95);

}

// Test that parameter reading fails when the file does not exist
BOOST_AUTO_TEST_CASE(readParametersFailWhenNoFile)
{

    // Create parameters
    Parameters pars;

    // Try to read a non-existing file
    tst::checkError([&]() {
        pars.read("nonexistent.txt");
    }, "Unable to open file nonexistent.txt");

}

// Test that parameter reading fails when invalid parameters are provided
BOOST_AUTO_TEST_CASE(readParametersFailWhenInvalidParameters)
{

    // Create parameters
    Parameters pars;

    // Write a file with invalid parameters
    tst::write("parameters.txt", "popsize 10\nselfing 0.95\ninvalid 42\n");

    // Try to read the file
    tst::checkError([&]() {
        pars.read("parameters.txt");
    }, "Invalid parameter name: invalid");

}

// Test that parameter reading fails when a value could not be read in
BOOST_AUTO_TEST_CASE(readParametersFailWhenValueCannotBeRead)
{

    // Create parameters
    Parameters pars;

    // Write a file with invalid value
    tst::write("parameters.txt", "popsize 10\nselfing invalid\n");

    // Try to read the file
    tst::checkError([&]() {
        pars.read("parameters.txt");
    }, "Could not read value for parameter: selfing");

}

// Test that error when initial population size is zero
BOOST_AUTO_TEST_CASE(checkFailsWhenInitialPopSizeIsZero) {
    Parameters pars;
    tst::write("parameters.txt", "popsize 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Initial population size cannot be zero");
}

// Test that error when zero demes
BOOST_AUTO_TEST_CASE(checkFailsWhenZeroDemes) {
    Parameters pars;
    tst::write("parameters.txt", "pgood 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "There cannot be zero demes");
}

// Test that the proportion of good patches should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenPropGoodPatchesNotBetweenZeroAndOne) {
    Parameters pars;
    tst::write("parameters.txt", "pgood 2 -0.1 1\npgoodEnd 0.1 0.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Proportion of good patches should be between zero and one");
    tst::write("parameters.txt", "pgood 2 0.1 1.1\npgoodEnd 0.1 0.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Proportion of good patches should be between zero and one");
}

// Test that the proportion of good patches after warming should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenPropGoodPatchesAfterWarmingNotBetweenZeroAndOne) {
    Parameters pars;
    tst::write("parameters.txt", "pgood 2 0.1 0.1\npgoodEnd -0.1 1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Proportion of good patches after warming should be between zero and one");
    tst::write("parameters.txt", "pgood 2 0.1 0.1\npgoodEnd 0.1 1.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Proportion of good patches after warming should be between zero and one");
}

// Test that carrying capacity must be positive
BOOST_AUTO_TEST_CASE(checkFailsWhenCarryingCapacityNotPositive) {

    Parameters pars;
    tst::write("parameters.txt", "capacities -1.0 100.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Carrying capacity must be positive");
    tst::write("parameters.txt", "capacities 0.0 100.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Carrying capacity must be positive");

}

// Test that carrying capacity after warming cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenCarryingCapacityAfterWarmingNotPositive) {

    Parameters pars;
    tst::write("parameters.txt", "capacitiesEnd -1.0 100.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Carrying capacity after warming must be positive");
    tst::write("parameters.txt", "capacitiesEnd 0.0 100.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Carrying capacity after warming must be positive");

}

// Test that stress level cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenStressLevelNegative) {
    Parameters pars;
    tst::write("parameters.txt", "stress -1.0 0.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Stress level cannot be negative");
}

// Test that stress level after warming cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenStressLevelAfterWarmingNegative) {
    Parameters pars;
    tst::write("parameters.txt", "stressEnd -1.0 0.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Stress level after warming cannot be negative");
}

// Test that maximum growth rate cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenMaxGrowthRateNegative) {
    Parameters pars;
    tst::write("parameters.txt", "maxgrowth -1.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Maximum growth rate cannot be negative");
}

// Test that steepness of the tolerance function cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenSteepnessNegative) {
    Parameters pars;
    tst::write("parameters.txt", "steep -1.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Steepness of the tolerance function cannot be negative");
}

// Test that dispersal rate should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenDispersalRateOutOfBounds) {
    Parameters pars;
    tst::write("parameters.txt", "dispersal 1.2");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Dispersal rate should be between zero and one");
    tst::write("parameters.txt", "dispersal -0.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Dispersal rate should be between zero and one");
}

// Test that mutation rate should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenMutationRateOutOfBounds) {
    Parameters pars;
    tst::write("parameters.txt", "mutation 1.2");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Mutation rate should be between zero and one");
    tst::write("parameters.txt", "mutation -0.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Mutation rate should be between zero and one");
}

// Test that there cannot be more than 1000 loci
BOOST_AUTO_TEST_CASE(checkFailsWhenTooManyLoci) {
    Parameters pars;
    tst::write("parameters.txt", "nloci 1001");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "There cannot be more than 1000 loci");
}

// Test that there cannot be zero chromosomes
BOOST_AUTO_TEST_CASE(checkFailsWhenZeroChromosomes) {
    Parameters pars;
    tst::write("parameters.txt", "nchrom 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "There cannot be zero chromosomes");
}

// Test that there cannot be zero loci
BOOST_AUTO_TEST_CASE(checkFailsWhenZeroLoci) {
    Parameters pars;
    tst::write("parameters.txt", "nloci 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "There cannot be zero loci");
}

// Test that initial allele frequency should be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenAlleleFrequencyOutOfBounds) {
    Parameters pars;
    tst::write("parameters.txt", "allfreq 1.2");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Initial allele frequency should be between zero and one");
    tst::write("parameters.txt", "allfreq -0.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Initial allele frequency should be between zero and one");
}

// Test that trade-off cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenTradeOffNegative) {
    Parameters pars;
    tst::write("parameters.txt", "tradeoff -1.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Trade-off cannot be negative");
}

// Test that rate of selfing must be between zero and one
BOOST_AUTO_TEST_CASE(checkFailsWhenSelfingRateOutOfBounds) {
    Parameters pars;
    tst::write("parameters.txt", "selfing 1.2");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Rate of selfing must be between zero and one");
    tst::write("parameters.txt", "selfing -0.1");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Rate of selfing must be between zero and one");
}

// Test that recombination rate cannot be negative
BOOST_AUTO_TEST_CASE(checkFailsWhenRecombinationRateNegative) {
    Parameters pars;
    tst::write("parameters.txt", "recombination -1.0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Recombination rate cannot be negative");
}

// Test that simulation time cannot be zero
BOOST_AUTO_TEST_CASE(checkFailsWhenSimulationTimeZero) {
    Parameters pars;
    tst::write("parameters.txt", "tend 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Simulation time cannot be zero");
}

// Test that save time interval cannot be zero
BOOST_AUTO_TEST_CASE(checkFailsWhenSaveTimeIntervalZero) {
    Parameters pars;
    tst::write("parameters.txt", "tsave 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Cannot save data every zero time point");
}

// Test that type should be 1 or 2
BOOST_AUTO_TEST_CASE(checkFailsWhenTypeOutOfBounds) {
    Parameters pars;
    tst::write("parameters.txt", "type 0");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Type should be 1 or 2");
    tst::write("parameters.txt", "type 3");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Type should be 1 or 2");
}

// Test that trade-off should be between 0 and 1 if type is 2
BOOST_AUTO_TEST_CASE(checkFailsWhenTradeOffOutOfBoundsForType2) {
    Parameters pars;
    tst::write("parameters.txt", "type 2\ntradeoff 1.2");
    tst::checkError([&]{ pars.read("parameters.txt"); }, "Trade-off should be between 0 and 1 if type is 2");
}

// Test that the parameter saving function works
BOOST_AUTO_TEST_CASE(parameterSavingWorks) {

    // Create parameters
    Parameters pars1;
    Parameters pars2;

    // Introduce some difference
    pars1.nloci = 42u;
    pars2.nloci = 999u;

    // Save one of them
    pars1.save("parameters.txt");

    // Overwrite the other
    pars2.read("parameters.txt");

    // Make sure the values have been properly replaced
    BOOST_CHECK_EQUAL(pars2.nloci, 42u);

}

// Test the updating of parameters under climate change
BOOST_AUTO_TEST_CASE(parameterUpdateUnderClimateChange) {

    // Create parameters
    Parameters pars;

    // Specify some parameter values
    pars.pgood = {0.5, 0.5, 0.6};
    pars.stress = {0, 1.0};
    pars.capacities = {100.0, 100.0};
    
    // Some end parameter values
    pars.pgoodEnd = {0.4, 0.4, 0.4};
    pars.stressEnd = {1.0, 2.0};
    pars.capacitiesEnd = {50.0, 50.0};

    // Climate change should take two generations
    pars.tend = 100;
    pars.tchange = 98;
    pars.twarming = 2;

    // Some parameter that should remain constant
    pars.steep = 0.2;

    // Pretend we are halfway through climate change
    pars.update(99);

    // Climate-related parameters should have moved halfway to their final value
    BOOST_CHECK_EQUAL(pars.pgood[0u], 0.45);
    BOOST_CHECK_EQUAL(pars.pgood[1u], 0.45);
    BOOST_CHECK_EQUAL(pars.pgood[2u], 0.5);
    BOOST_CHECK_EQUAL(pars.stress[0u], 0.5);
    BOOST_CHECK_EQUAL(pars.stress[1u], 1.5);
    BOOST_CHECK_EQUAL(pars.capacities[0u], 75.0);
    BOOST_CHECK_EQUAL(pars.capacities[1u], 75.0);

    // One more time step
    pars.update(100);

    // Now those parameters should have reached their final values
    BOOST_CHECK_EQUAL(pars.pgood[0u], pars.pgoodEnd[0u]);
    BOOST_CHECK_EQUAL(pars.pgood[1u], pars.pgoodEnd[1u]);
    BOOST_CHECK_EQUAL(pars.pgood[2u], pars.pgoodEnd[2u]);
    BOOST_CHECK_EQUAL(pars.stress[0u], pars.stressEnd[0u]);
    BOOST_CHECK_EQUAL(pars.stress[1u], pars.stressEnd[1u]);
    BOOST_CHECK_EQUAL(pars.capacities[0u], pars.capacitiesEnd[0u]);
    BOOST_CHECK_EQUAL(pars.capacities[1u], pars.capacitiesEnd[1u]);

    // Constant parameters should have remained constant
    BOOST_CHECK_EQUAL(pars.steep, 0.2);

}