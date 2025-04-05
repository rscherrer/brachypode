#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// These are tests that have to do with the Population class.

#include "testutils.hpp"
#include "../src/population.hpp"
#include <boost/test/unit_test.hpp>

// Test that a population initializes properly
BOOST_AUTO_TEST_CASE(populationInitializesProperly) {

    // Parameters
    Parameters pars;

    // Make sure this is false
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Check that individuals have been created
    BOOST_CHECK_EQUAL(pop.size(), pars.popsize);

    // Prepare to sum
    size_t sum = 0u;

    // Sum all demes
    for (size_t i = 0u; i < pop.size(); ++i)
        sum += pop.getDeme(i);

    // Check that all individuals are in the first deme
    BOOST_CHECK_EQUAL(sum, 0u);

}

// Test when a population does not sow (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationSowsAtRandom) {

    // Parameters
    Parameters pars;

    // Sow at random
    pars.sow = true;

    // Make sure there are many
    pars.popsize = 1000u;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Prepare to sum
    size_t sum = 0u;

    for (size_t i = 0u; i < pop.size(); ++i) 
        sum += pop.getDeme(i);

    // Check that individuals are scattered
    BOOST_CHECK(sum > 0u);

}

// Test screen output
BOOST_AUTO_TEST_CASE(populationScreenOutput) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.popsize = 10u;
    pars.pgood = {1.0, 1.0, 1.0};
    pars.verbose = true;

    // Architecture
    Architecture arch(pars);

    // Population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Check that output is as expected
    tst::checkOutput([&] { pop.cycle(print); }, "n = { 10 0 0 } at t = 0\n");

}

// Moving on works
BOOST_AUTO_TEST_CASE(populationMovesOn) {

    // Parameters
    Parameters pars;

    // Architecture
    Architecture arch(pars);

    // Population
    Population pop(pars, arch);

    // Check time is zero
    BOOST_CHECK_EQUAL(pop.getTime(), 0);

    // Increment time
    pop.moveon();

    // Check time has changed
    BOOST_CHECK_EQUAL(pop.getTime(), 1);

}

// Test parameter update under climate change
BOOST_AUTO_TEST_CASE(populationUpdateClimateChange) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.tchange = 0u;
    pars.twarming = 2u;
    pars.stress = { 0.0, 5.0 };
    pars.stressEnd = { 1.0, 6.0 };
    pars.capacities = { 50.0, 100.0 };
    pars.capacitiesEnd = { 20.0, 50.0 };
    pars.pgood = { 0.1, 0.5 };
    pars.pgoodEnd = { 0.1, 0.4 }; 

    // Architecture
    Architecture arch(pars);

    // Population
    Population pop(pars, arch);

    // Go through one time step
    pop.moveon();

    // Update parameters
    pop.update();

    // Values should be halfway through final change
    BOOST_CHECK_EQUAL(pop.getPGood(0u), 0.1);
    BOOST_CHECK_EQUAL(pop.getPGood(1u), 0.45);
    BOOST_CHECK_EQUAL(pop.getStress(0u), 0.5);
    BOOST_CHECK_EQUAL(pop.getStress(1u), 5.5);
    BOOST_CHECK_EQUAL(pop.getCapacity(0u), 35.0);
    BOOST_CHECK_EQUAL(pop.getCapacity(1u), 75.0);

    // One more time
    pop.moveon();

    // Update
    pop.update();

    // Final values should be reached
    BOOST_CHECK_EQUAL(pop.getPGood(0u), pars.pgoodEnd[0u]);
    BOOST_CHECK_EQUAL(pop.getPGood(1u), pars.pgoodEnd[1u]);
    BOOST_CHECK_EQUAL(pop.getStress(0u), pars.stressEnd[0u]);
    BOOST_CHECK_EQUAL(pop.getStress(1u), pars.stressEnd[1u]);
    BOOST_CHECK_EQUAL(pop.getCapacity(0u), pars.capacitiesEnd[0u]);
    BOOST_CHECK_EQUAL(pop.getCapacity(1u), pars.capacitiesEnd[1u]);

}

// Test that the population goes extinct when expected (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationGoesExtinct) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.maxgrowth = 0.0;
    pars.tradeoff = 100.0;
    pars.allfreq = 1.0;
    pars.effect = 0.1;
    pars.pgood = { 0.0, 0.0 };
    pars.pgoodEnd = { 0.0, 0.0 };

    // Note: Since the demographics follow Ricker dynamics we cannot
    // guarantee that the number of offspring will be zero, as this
    // number is an exponential, but we can try to make what goes into
    // that exponential very negative (e.g. population with high trait
    // value and strong trade-off), to make the expected number of offspring
    // very close to zero. Still, there will be a nonzero chance that the
    // population does not go extinct in one generation.

    // Architecture
    Architecture arch(pars);

    // Population
    Population pop(pars, arch);

    // Dummy printer
    Printer print({"foo", "bar"}); 

    // One time step
    pop.cycle(print);

    // Check that the population went extinct
    BOOST_CHECK(pop.extinct());

}

// Test change in population size (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationChangesSize) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.popsize = 10u;
    pars.maxgrowth = 2.0;

    // Architecture
    Architecture arch(pars);

    // Population
    Population pop(pars, arch);

    // Dummy printer
    Printer print({"foo", "bar"}); 

    // Record population size 
    const size_t oldsize = pop.size();

    // One time step
    pop.cycle(print);

    // The population should have changed size
    BOOST_CHECK(pop.size() != oldsize);

}

// Test that integration with printer works
BOOST_AUTO_TEST_CASE(populationCanPrint) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.popsize = 3u;
    pars.pgood = {0.0, 0.0};
    pars.pgoodEnd = {0.0, 0.0};
    pars.allfreq = 1.0;
    pars.nloci = 5u;
    pars.effect = 0.1;
    pars.tsave = 1u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Population
    Population pop(pars, arch);

    // Printer
    Printer print({"time", "popsize", "patchsizes", "traitmeans", "individuals"});

    // Open the printer
    print.open();

    // Population cycle
    pop.cycle(print);

    // Close the printer
    print.close();

    // Read values back in
    std::vector<double> time = tst::read("time.dat");
    std::vector<double> popsize = tst::read("popsize.dat");
    std::vector<double> patchsizes = tst::read("patchsizes.dat");
    std::vector<double> traitmeans = tst::read("traitmeans.dat");
    std::vector<double> individuals = tst::read("individuals.dat");

    // Check sizes
    BOOST_CHECK_EQUAL(time.size(), 1u);
    BOOST_CHECK_EQUAL(popsize.size(), 1u);
    BOOST_CHECK_EQUAL(patchsizes.size(), 4u);
    BOOST_CHECK_EQUAL(traitmeans.size(), 4u);
    BOOST_CHECK_EQUAL(individuals.size(), 9u);

    // Check time values
    BOOST_CHECK_EQUAL(time[0u], 0.0);

    // Check population size
    BOOST_CHECK_EQUAL(popsize[0u], 3.0);

    // All individuals should be in facilitated patch of first deme
    BOOST_CHECK_EQUAL(patchsizes[0u], 0.0);
    BOOST_CHECK_EQUAL(patchsizes[1u], 3.0);
    BOOST_CHECK_EQUAL(patchsizes[2u], 0.0);
    BOOST_CHECK_EQUAL(patchsizes[3u], 0.0);

    // Five loci and allele frequency of one
    BOOST_CHECK_EQUAL(traitmeans[0u], 0.0);
    BOOST_CHECK_EQUAL(traitmeans[1u], 0.5);
    BOOST_CHECK_EQUAL(traitmeans[2u], 0.0);
    BOOST_CHECK_EQUAL(traitmeans[3u], 0.0);

    // Check individuals are in the first deme
    BOOST_CHECK_EQUAL(individuals[0u], 0.0);
    BOOST_CHECK_EQUAL(individuals[3u], 0.0);
    BOOST_CHECK_EQUAL(individuals[6u], 0.0);

    // Check individuals are in the facilitated patch
    BOOST_CHECK_EQUAL(individuals[1u], 1.0);
    BOOST_CHECK_EQUAL(individuals[4u], 1.0);
    BOOST_CHECK_EQUAL(individuals[7u], 1.0);

    // Check individual trait values
    BOOST_CHECK_EQUAL(individuals[2u], 0.5);
    BOOST_CHECK_EQUAL(individuals[5u], 0.5);
    BOOST_CHECK_EQUAL(individuals[8u], 0.5);

    // Remove files
    std::remove("time.dat");
    std::remove("popsize.dat");
    std::remove("patchsizes.dat");
    std::remove("traitmeans.dat");
    std::remove("individuals.dat");

}

// Test that population with non-linear trade-off works
BOOST_AUTO_TEST_CASE(populationWithNonLinearTradeOff) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.tradeoff = 0.5;
    pars.nonlinear = 0.5;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Cycle
    BOOST_CHECK_NO_THROW(pop.cycle(print));

}

// Test growth rate computation
BOOST_AUTO_TEST_CASE(growthRateComputation) {

    // Linear trade-off
    BOOST_CHECK_EQUAL(pop::growth(0.0, 10.0, 2.0, 0.1, 1.0), 2.0);
    BOOST_CHECK_EQUAL(pop::growth(5.0, 10.0, 2.0, 0.1, 1.0), 1.5);
    BOOST_CHECK_EQUAL(pop::growth(10.0, 10.0, 2.0, 0.1, 1.0), 1.0);

    // Concave
    BOOST_CHECK_EQUAL(pop::growth(0.0, 10.0, 2.0, 0.1, 2.0), 2.0);
    BOOST_CHECK_EQUAL(pop::growth(5.0, 10.0, 2.0, 0.1, 2.0), 1.75);
    BOOST_CHECK_EQUAL(pop::growth(10.0, 10.0, 2.0, 0.1, 2.0), 1.0);

    // Convex
    BOOST_CHECK_EQUAL(pop::growth(0.0, 10.0, 2.0, 0.1, 0.5), 2.0);
    BOOST_CHECK_EQUAL(pop::growth(2.5, 10.0, 2.0, 0.1, 0.5), 1.5);
    BOOST_CHECK_EQUAL(pop::growth(10.0, 10.0, 2.0, 0.1, 0.5), 1.0);

    // No trade-off
    BOOST_CHECK_EQUAL(pop::growth(5.0, 10.0, 2.0, 0.0, 0.5), 2.0);
    BOOST_CHECK_EQUAL(pop::growth(5.0, 10.0, 2.0, 0.0, 1.0), 2.0);
    BOOST_CHECK_EQUAL(pop::growth(5.0, 10.0, 2.0, 0.0, 2.0), 2.0);

}

// Test fitness computation
BOOST_AUTO_TEST_CASE(fitnessComputation) {

    // Fitness is one when growth rate is zero
    BOOST_CHECK_EQUAL(pop::ricker(1.0, 0.0, 100.0), 1.0);

    // Fitness is one when at carrying capacity
    BOOST_CHECK_EQUAL(pop::ricker(100.0, 1.0, 100.0), 1.0);

    // Fitness is the (exponential of the) growth rate when very very far from capacity
    BOOST_CHECK_EQUAL(pop::ricker(0.0, 1.1, 100.0), std::exp(1.1));

}

// Test survival probability function
BOOST_AUTO_TEST_CASE(survivalFunction) {

    // Survival should be one when stress is zero
    BOOST_CHECK_EQUAL(pop::survival(10.0, 0.0, 5.0), 1.0);

    // Survival very low when stress is high
    BOOST_CHECK(pop::survival(0.1, 5.0, 5.0) < 1E-6);

    // But high if tolerance is also high
    BOOST_CHECK_CLOSE(pop::survival(10.0, 5.0, 5.0), 1.0, 1E-6);

    // Survival is exactly half when at inflection point
    BOOST_CHECK_EQUAL(pop::survival(10.0, 10.0, 1.0), 0.5);
    BOOST_CHECK_EQUAL(pop::survival(10.0, 10.0, 2.0), 0.5);
    BOOST_CHECK_EQUAL(pop::survival(10.0, 10.0, 5.0), 0.5);

    // Compute probabilities just before inflection
    const double s1b = pop::survival(11.0, 10.0, 1.0);
    const double s2b = pop::survival(11.0, 10.0, 2.0);
    const double s3b = pop::survival(11.0, 10.0, 5.0);

    // Just after
    const double s1a = pop::survival(9.0, 10.0, 1.0);
    const double s2a = pop::survival(9.0, 10.0, 2.0);
    const double s3a = pop::survival(9.0, 10.0, 5.0);

    // Survival should go down faster with steeper curve
    BOOST_CHECK(s1b < s2b);
    BOOST_CHECK(s2b < s3b);
    BOOST_CHECK(s1a > s2a);
    BOOST_CHECK(s2a > s3a);

}

// Population cycle with dispersal (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationCycleWithDispersal) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 1.0;
    pars.pgood = { 1.0, 1.0 };
    pars.pgoodEnd = { 1.0, 1.0 };
    pars.popsize = 2u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getDeme(0u) == 0u);
    assert(pop.getDeme(1u) == 0u);

    // Cycle
    pop.cycle(print);

    // Note: the population could be extinct

    // Check that dispersal has occurred
    BOOST_CHECK_EQUAL(pop.getDeme(0u), 1u);

}

// Population cycle with no dispersal (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationCycleWithNoDispersal) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 0.0;
    pars.pgood = { 1.0, 1.0 };
    pars.pgoodEnd = { 1.0, 1.0 };
    pars.popsize = 2u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getDeme(0u) == 0u);
    assert(pop.getDeme(1u) == 0u);

    // Cycle
    pop.cycle(print);

    // Note: the population could be extinct

    // Check that no dispersal has occurred
    BOOST_CHECK_EQUAL(pop.getDeme(0u), 0u);

}

// Population cycle with no dispersal cause one deme (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationCycleWithNoDispersalCuzOneDeme) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 1.0;
    pars.pgood = { 1.0 };
    pars.pgoodEnd = { 1.0 };
    pars.popsize = 2u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getDeme(0u) == 0u);
    assert(pop.getDeme(1u) == 0u);

    // Cycle
    pop.cycle(print);

    // Note: the population could be extinct

    // Check that no dispersal has occurred
    BOOST_CHECK_EQUAL(pop.getDeme(0u), 0u);

}

// Test when seeds all land in unfacilitated patch
BOOST_AUTO_TEST_CASE(populationCycleWithZeroPatchCover) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 1.0;
    pars.pgood = { 1.0, 0.0 };
    pars.pgoodEnd = { 1.0, 0.0 };
    pars.popsize = 100u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getPatch(0u) == 1u);

    // Cycle
    pop.cycle(print);

    // Note: the population could be extinct

    // Check patch
    BOOST_CHECK_EQUAL(pop.getPatch(0u), 0u);

}

// Test when seeds all land in facilitated patch
BOOST_AUTO_TEST_CASE(populationCycleWithFullPatchCover) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 1.0;
    pars.pgood = { 1.0, 1.0 };
    pars.pgoodEnd = { 1.0, 1.0 };
    pars.popsize = 100u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getPatch(0u) == 1u);

    // Cycle
    pop.cycle(print);

    // Note: the population could be extinct

    // Check patch
    BOOST_CHECK_EQUAL(pop.getPatch(0u), 1u);

}

// Test that extinction occurs when too harsh to survive
BOOST_AUTO_TEST_CASE(populationCycleTooHarshForSurvival) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 1.0;
    pars.pgood = { 0.0 };
    pars.pgoodEnd = { 0.0 };
    pars.popsize = 10u;
    pars.sow = false;

    // Architecture
    Architecture arch(pars);

    // Create a population
    Population pop(pars, arch);

    // Printer
    Printer print({"foo", "bar"});

    // Cycle
    pop.cycle(print);

    // Note: the population could be not extinct

    // Check patch
    BOOST_CHECK_EQUAL(pop.size(), 0u);

}