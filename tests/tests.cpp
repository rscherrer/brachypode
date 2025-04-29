#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test all the uses and misuses of the program. These mostly have to
// with calling the program, passing arguments, reading from and writing to
// files, and error handling.

#include "testutils.hpp"
#include "../src/MAIN.hpp"
#include <boost/test/unit_test.hpp>

// Test that the simulation runs
BOOST_AUTO_TEST_CASE(useCase) {

    // Check that the program runs
    BOOST_CHECK_NO_THROW(doMain({"program"}));

}

// Test that it fails when too many arguments are provided
BOOST_AUTO_TEST_CASE(abuseTooManyArgs) {

    // Check error
    tst::checkError([&] { 
        doMain({"program", "parameter.txt", "onetoomany.txt"});
    }, "Too many arguments provided");

}

// Test that the simulation runs with a parameter file
BOOST_AUTO_TEST_CASE(useCaseWithParameterFile) {

    // Write a parameter file
    tst::write("parameters.txt", "popsize 9");

    // Check that the program runs
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // Cleanup
    std::remove("parameters.txt");

}

// Test that error when invalid parameter file
BOOST_AUTO_TEST_CASE(abuseInvalidParameterFile) {

    // Check error
    tst::checkError([&] {
        doMain({"program", "nonexistent.txt"});
    }, "Unable to open file nonexistent.txt");

}

// Test that it works when reading an architecture
BOOST_AUTO_TEST_CASE(useCaseWithArchitectureLoading) {

    // Write a parameter file specifying to load architecture
    tst::write("parameters.txt", "loadarch 1");

    // Write an achitecture file
    tst::write("architecture.txt", "nloci 3\nlocations 0.1 0.2 0.3\neffects 0.1 0.1 0.1\n");

    // Check that the simulation runs
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // Note: we do not check that the architecture was properly loaded here.
    // That stuff is tested in the relevant test file for architectures.
    // Same for other tests below: we only focus on the high-level
    // behavior of the program in this file.

    // Cleanup
    std::remove("parameters.txt");
    std::remove("architecture.txt");

}

// Test that error when architecture file not found
BOOST_AUTO_TEST_CASE(abuseArchitectureFileNotFound) {

    // Write a parameter file specifying to load the architecture
    tst::write("parameters.txt", "loadarch 1");

    // Check that error if no architecture was created
    tst::checkError([&] {
        doMain({"program", "parameters.txt"});
    }, "Unable to open file architecture.txt");

    // Cleanup
    std::remove("parameters.txt");

}

// Test that it works when saving the architecture
BOOST_AUTO_TEST_CASE(useCaseWithArchitectureSaving) {

    // Write a parameter file specifying to save the architecture
    tst::write("parameters.txt", "savearch 1");
    
    // Run a simulation
    doMain({"program", "parameters.txt"});

    // Update parameter file to only read provided architecture
    tst::write("parameters.txt", "loadarch 1\nsavearch 0");

    // New simulation should read the architecture if present
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // Cleanup
    std::remove("parameters.txt");
    std::remove("architecture.txt");

}

// Test that it works when saving the parameters
BOOST_AUTO_TEST_CASE(useCaseWithParameterSaving) {

    // Write a parameter file specifying to save parameters
    tst::write("parameters.txt", "savepars 1");

    // Run a simulation
    doMain({"program", "parameters.txt"});

    // Read the saved parameters
    Parameters pars1("paramlog.txt");

    // Rerun the simulation with the saved parameters
    doMain({"program", "paramlog.txt"});

    // Read the overwritten parameters
    Parameters pars2("paramlog.txt");

    // Check that the (clock-generated) seed is the same
    BOOST_CHECK_EQUAL(pars1.seed, pars2.seed);

    // Cleanup
    std::remove("parameters.txt");
    std::remove("paramlog.txt");

}

// Test that it works when the user can choose which data to save
BOOST_AUTO_TEST_CASE(useCaseUserDefinedOutput) {

    // Write an output request file
    tst::write("whattosave.txt", "time");

    // Write a parameter file
    tst::write("parameters.txt", "savedat 1\nchoose 1");

    // Pre-emptive cleanup
    std::remove("traitmeans.dat");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Read the data if they exist
    const std::vector<double> values = tst::read("time.dat");

    // Check
    assert(!values.empty());

    // Check their values
    for (auto i : values)
        BOOST_CHECK_EQUAL(values[i], i);
    
    // Check that no other output file was read
    tst::checkError([&] { 
        tst::read("traitmeans.dat"); 
    }, "Unable to open file traitmeans.dat");
    
    // Cleanup
    std::remove("parameters.txt");
    std::remove("whattosave.txt");
    std::remove("time.dat");
    
}

// Test that error when wrong output request file
BOOST_AUTO_TEST_CASE(abuseWrongOutputRequestFile) {

    // Write a parameter file
    tst::write("parameters.txt", "savedat 1\nchoose 1");

    // Check error if output request file not present
    tst::checkError([&] {
        doMain({"program", "parameters.txt"});
    }, "Unable to open file whattosave.txt");

    // Cleanup
    std::remove("parameters.txt");

}

// Test that all outputs are saved if no choice is made
BOOST_AUTO_TEST_CASE(useCaseAllOutputsIfNoChoice) {

    // Write a parameter file with data saving but no choice
    tst::write("parameters.txt", "ndemes 2\npgood 0.8 0.8\npgoodEnd 0.1 0.1\ntend 10\ntsave 1\nsavedat 1\nchoose 0");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Check that all output files can be read
    BOOST_CHECK_NO_THROW(tst::read("time.dat"));
    BOOST_CHECK_NO_THROW(tst::read("popsize.dat"));
    BOOST_CHECK_NO_THROW(tst::read("patchsizes.dat"));
    BOOST_CHECK_NO_THROW(tst::read("traitmeans.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individuals.dat"));

    // Check some values
    BOOST_CHECK_EQUAL(tst::read("time.dat").size(), 11u);
    BOOST_CHECK_EQUAL(tst::read("popsize.dat").size(), 11u);
    BOOST_CHECK_EQUAL(tst::read("patchsizes.dat").size(), 44u);
    BOOST_CHECK_EQUAL(tst::read("traitmeans.dat").size(), 44u);

    // Read the saved population sizes
    const std::vector<double> popsizes = tst::read("popsize.dat");

    // Prepare to sum
    size_t n = 0.0;

    // Check
    assert(!popsizes.empty());

    // Sum over generations
    for (auto popsize : popsizes) n += static_cast<size_t>(popsize);

    // Check size
    BOOST_CHECK_EQUAL(tst::read("individuals.dat").size(), 3u * n);

    // Cleanup
    std::remove("parameters.txt");
    std::remove("time.dat");
    std::remove("popsize.dat");
    std::remove("patchsizes.dat");
    std::remove("traitmeans.dat");
    std::remove("individuals.dat");

}

// Test that nothing is saved if no data saving
BOOST_AUTO_TEST_CASE(useCaseNothingIsSaved) {

    // Write a parameter file specifying no data saving
    tst::write("parameters.txt", "savedat 0");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Check that none of the possible output files are present
    tst::checkError([&] {tst::read("time.dat");}, "Unable to open file time.dat");
    tst::checkError([&] {tst::read("popsize.dat");}, "Unable to open file popsize.dat");
    tst::checkError([&] {tst::read("patchsizes.dat");}, "Unable to open file patchsizes.dat");
    tst::checkError([&] {tst::read("traitmeans.dat");}, "Unable to open file traitmeans.dat");
    tst::checkError([&] {tst::read("individuals.dat");}, "Unable to open file individuals.dat");

    // Cleanup
    std::remove("parameters.txt");

}

// Test that it works when sowing individuals at random at the start
BOOST_AUTO_TEST_CASE(useCaseSowingIndividualsAtRandom) {

    // Write a parameter file
    tst::write("parameters.txt", "sow 1");

    // Check that no error
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // Cleanup
    std::remove("parameters.txt");

}

// Test that it works when verbose is on
BOOST_AUTO_TEST_CASE(useCaseWithVerbose) {

    // Write a parameter file with verbose
    tst::write("parameters.txt", "verbose 1\ntend 1\npopsize 10\nndemes 3\npgood 1 1 1\npgoodEnd 0 0 0");

    // Capture output
    const std::string output = tst::captureOutput([&] { doMain({"program", "parameters.txt"}); });

    // Find relevant bits in output
    BOOST_CHECK(output.find("Simulation started") != std::string::npos);
    BOOST_CHECK(output.find("n = { 10 0 0 } at t = 0") != std::string::npos);
    BOOST_CHECK(output.find("Simulation ended") != std::string::npos);

    // Cleanup
    std::remove("parameters.txt");

}

// Test that extinction is handled well (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(useCaseWithExtinction) {

    // Write a parameter file doomed to go extinct in one generation
    tst::write("parameters.txt", "popsize 10\nmaxgrowth 0\ntradeoff 100\nallfreq 0\nndemes 1\npgood 0.0\npgoodEnd 0.0\neffect 0.001\nnloci 100");

    // Note: there is a nonzero chance that the population does not
    // go extinct.

    // Run the simulation
    const std::string output = tst::captureOutput([&] { doMain({"program", "parameters.txt"}); });

    // Check for the relevant bit
    BOOST_CHECK(output.find("Population went extinct at t = 0") != std::string::npos);

    // Cleanup
    std::remove("parameters.txt");

}