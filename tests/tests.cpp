#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test all the uses and misuses of the program. These mostly have to
// with calling the program, passing arguments, reading from and writing to
// files, and error handling.

#include "testutils.hpp"
#include "../src/MAIN.hpp"
#include <boost/test/unit_test.hpp>

// TODO: Check screen output?

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

    // TODO: Maybe try with all the flags off

    // Write a parameter file
    tst::write("parameters.txt", "popsize 9");

    // Check that the program runs
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

}

// Test that error when invalid parameter file
BOOST_AUTO_TEST_CASE(abuseInvalidParameterFile) {

    // Check error
    tst::checkError([&] {
        doMain({"program", "nonexistent.txt"});
    }, "Unable to open parameters.txt");

}

// Test that screen output can be rerouted properly
BOOST_AUTO_TEST_CASE(useCaseWithScreenToLog) {

    // Write a parameter file specifying to reroute
    tst::write("parameters.txt", "savelog 1");

    // Run the program
    doMain({"program", "parameters.txt"});

    // Read the log file
    const std::string log = tst::readtext("log.txt");

    // Check screen output
    BOOST_CHECK_EQUAL(log, "Hello!");

}

// Test that it works when reading an architecture
BOOST_AUTO_TEST_CASE(useCaseWithArchitectureLoading) {

    // Write a parameter file specifying to load architecture
    tst::write("parameters.txt", "loadarch 1");

    // Write an achitecture file
    tst::write("architecture.txt", "1\n1\n3\n0.1 0.2 0.3\n0.1 0.1 0.1\n");

    // Check that the simulation runs
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // Note: we do not check that the architecture was properly loaded here.
    // That stuff is tested in the relevant test file for architectures.
    // Same for other tests below: we only focus on the high-level
    // behavior of the program in this file.

}

// Test that error when architecture file not found
BOOST_AUTO_TEST_CASE(abuseArchitectureFileNotFound) {

    // Write a parameter file specifying to load the architecture
    tst::write("parameters.txt", "loadarch 1");

    // Check that error if no architecture was created
    tst::checkError([&] {
        doMain({"program", "parameters.txt"});
    }, "Unable to open file architecture.txt");

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

}

// Test that it works when saving the parameters
BOOST_AUTO_TEST_CASE(useCaseWithParameterSaving) {

    // Write a parameter file specifying to save parameters
    tst::write("parameters.txt", "savepars 1");

    // Run a simulation
    doMain({"program", "parameters.txt"});

    // Read the parameter log file
    const std::string pars = tst::readtext("paramlog.txt");

    // Check
    BOOST_CHECK_EQUAL(pars, "Hello!");

    // TODO: Check properly those things

}

// Test that it works when the user can choose which data to save
BOOST_AUTO_TEST_CASE(useCaseUserDefinedOutput) {

    // Write an output request file
    tst::write("whattosave.txt", "time");

    // Write a parameter file
    tst::write("parameters.txt", "savedat 1\nchoose 1");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Read the data if they exist
    const std::vector<double> values = tst::read("time.dat");

    // Check their values
    for (auto &i : values)
        BOOST_CHECK_EQUAL(values[i], i);
    
    // Check that no other output file was read
    tst::checkError([&] { 
        tst::read("traitmeans.dat"); 
    }, "Unable to open file traitmeans.dat");
    
}

// Test that error when wrong output request file
BOOST_AUTO_TEST_CASE(abuseWrongOutputRequestFile) {

    // Write a parameter file
    tst::write("parameters.txt", "savedat 1\nchoose 1");

    // Check error if output request file not present
    tst::checkError([&] {
        doMain({"program", "parameters.txt"});
    }, "Unable to open file whattosave.txt");

}

// Test that all outputs are saved if no choice is made
BOOST_AUTO_TEST_CASE(useCaseAllOutputsIfNoChoice) {

    // Write a parameter file with data saving but no choice
    tst::write("parameters.txt", "savedat 1\nchoose 0");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Check that all output files can be read
    BOOST_CHECK_NO_THROW(tst::read("time.dat"));
    BOOST_CHECK_NO_THROW(tst::read("popsize.dat"));
    BOOST_CHECK_NO_THROW(tst::read("patchsizes.dat"));
    BOOST_CHECK_NO_THROW(tst::read("traitmeans.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individuals.dat"));

    // TODO: Do we want to bother with checking actual values here?

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

}

// Test that it works when sowing individuals at random at the start
BOOST_AUTO_TEST_CASE(useCaseSowingIndividualsAtRandom) {

    // Write a parameter file
    tst::write("parameters.txt", "sow 1");

    // Check that no error
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // TODO: If this was part of a class I could test its behavior properly
    // TODO: I could test that without sowing all individuals are in the same deme?

}

// Test that it works when verbose is on
BOOST_AUTO_TEST_CASE(useCaseWithVerbose) {

    // Write a parameter file with verbose and log file
    tst::write("parameters.txt", "verbose 1\nsavelog 1");

    // Run the program
    doMain({"program", "parameters.txt"});

    // Read the log file
    const std::string log = tst::readtext("log.txt");

    // Check
    BOOST_CHECK_EQUAL(log, "Hello!");

}

// Test that a type 2 simulation (non-linear trade-off) works
BOOST_AUTO_TEST_CASE(useCaseNonLinearTradeOff) {

    // Write a parameter file with non-linear trade-off
    tst::write("parameters.txt", "type 2");

    // TODO: Change that to nonlinear or something

    // Check that no error
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // TODO: Same, perhaps unnecessary if I had a class

}

// Test that extinction is handled well
BOOST_AUTO_TEST_CASE(useCaseWithExtinction) {

    // Write a parameter file doomed to go extinct in one generation
    tst::write("parameters.txt", "poopsize 1\nmaxgrowth 0\nsavelog 1");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Read the log file
    const std::string log = tst::readtext("log.txt");

    // Check
    BOOST_CHECK_EQUAL(log, "Hello!");

    // TODO: Same here, could do with a class

}