#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

// Here we test all the uses and misuses of the program. These mostly have to
// with calling the program, passing arguments, reading from and writing to
// files, and error handling.

#include "src/simulation.h"
#include <boost/test/unit_test.hpp>

// Test that the simulation runs
BOOST_AUTO_TEST_CASE(useCase) {

    // Check that the main simulation function works
    BOOST_CHECK_EQUAL(simulate({"program_name"}), 0);

}

// Test that it works when a parameter file is supplied
BOOST_AUTO_TEST_CASE(runWithParameterFile) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "popsize 10\n";
    file << "pgood 3 0.5 0.5 0.5\n";
    file << "maxgrowths 1.0 2.0\n";
    file << "xopts 5.0 1.0\n";
    file << "xwidths 1.0 3.0\n";
    file << "capacities 1000 100\n";
    file << "dispersal 0.1\n";
    file << "mutation 0.001\n";
    file << "nchrom 1\n";
    file << "nloci 20\n";
    file << "effect 0.3\n";
    file << "selfing 0.95\n";
    file << "recombination 1\n";
    file << "tend 100\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 0);

}

// Test that it fails when too many arguments are provided
BOOST_AUTO_TEST_CASE(tooManyArgs) {

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt", "arg2"}), 1);

}

// Test that it fails if the parameter file name is invalid
BOOST_AUTO_TEST_CASE(invalidFileName) {

    BOOST_CHECK_EQUAL(simulate({"program_name", "paraaameters.txt"}), 1);

}

// Should error when no demes are provided
BOOST_AUTO_TEST_CASE(errorWhenNoDeme) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "pgood 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when invalid parameter name
BOOST_AUTO_TEST_CASE(errorWhenInvalidParameterName) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "pgoooood 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when initial population size is zero
BOOST_AUTO_TEST_CASE(errorWhenInitPopSizeIsZero) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "popsize 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when proportion of good patches is not between zero and one
BOOST_AUTO_TEST_CASE(errorWhenPropGoodNotBetweenZeroAndOne) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "pgood 3 1.5 -4 2\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when maximum growth rate is negative
BOOST_AUTO_TEST_CASE(errorWhenMaxGrowthIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "maxgrowths 1 -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when niche width is negative
BOOST_AUTO_TEST_CASE(errorWhenNicheWidthIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "xwidth 1 -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when carrying capacity is negative
BOOST_AUTO_TEST_CASE(errorWhenCapacityIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "capacities 1 -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when dispersal is not between zero and one
BOOST_AUTO_TEST_CASE(errorWhenDispersalNotBetweenZeroAndOne) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "dispersal -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when mutation is not between zero and one
BOOST_AUTO_TEST_CASE(errorWhenMutationNotBetweenZeroAndOne) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "mutation -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when too many loci
BOOST_AUTO_TEST_CASE(errorWhenTooManyLoci) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "nloci 2000\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when zero chromosomes
BOOST_AUTO_TEST_CASE(errorWhenZeroChromosomes) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "nchrom 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when zero loci
BOOST_AUTO_TEST_CASE(errorWhenZeroLoci) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "nloci 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when selfing is not between zero and one
BOOST_AUTO_TEST_CASE(errorWhenSelfingNotBetweenZeroAndOne) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "selfing -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when recombination is negative
BOOST_AUTO_TEST_CASE(errorWhenRecombinationIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "recombination -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Should error when simulation time is zero
BOOST_AUTO_TEST_CASE(errorWhenSimulationTimeIsZero) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "tend 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that parameters are saved properly
BOOST_AUTO_TEST_CASE(paramSavedProperly) {

    // Prepare a parameter file
    std::ofstream file;
    file.open("parameters.txt");
    file << "savepars 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 0);

    // Second run where we provide the saved parameters from the first one
    BOOST_CHECK_EQUAL(simulate({"program_name", "paramlog.txt"}), 0);

    // Check the seed was saved too
    std::ifstream infile;
    infile.open("paramlog.txt");
    std::string input;
    size_t seed = 0u;
    while (infile >> input) if (input == "seed") infile >> seed;
    infile.close();

    BOOST_CHECK(seed > 0u); // check that a random (nonzero) seed was saved

}

// Test that architecture can be loaded
BOOST_AUTO_TEST_CASE(architectureFileLoadedProperly) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.1 0.2 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "savepars 1\n"; // make sure to save the parameters
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 0);

    // Read the numbers of chromosomes and loci from parameters that were saved
    std::ifstream infile;
    infile.open("paramlog.txt");
    std::string input;
    size_t nchrom, nloci;
    while (infile >> input) {
        if (input == "nchrom") infile >> nchrom;
        if (input == "nloci") infile >> nloci;
    }
    infile.close();

    // Check they are right
    BOOST_CHECK_EQUAL(nchrom, 3u);
    BOOST_CHECK_EQUAL(nloci, 4u);

}

// Should error if zero chromosomes in loaded architecture
BOOST_AUTO_TEST_CASE(errorWhenZeroChromosomesInArchitecture) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "0 0.1 0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.1 0.2 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error if zero chromosomes in loaded architecture
BOOST_AUTO_TEST_CASE(errorWhenChromosomeEndsInWrongOrder) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.33 0.1 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.1 0.2 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error when negative chromosomme ends
BOOST_AUTO_TEST_CASE(errorWhenNegativeChromosomeEnds) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 -0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.1 0.2 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error when the last chromosome end is not one
BOOST_AUTO_TEST_CASE(errorWhenLastChromosomeEndIsNotOne) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 0.33 0.9\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.1 0.2 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error when zero loci in loaded architecture
BOOST_AUTO_TEST_CASE(errorWhenZeroLociInArchitecture) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "0 0.1 0.2 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error when loci locations are not in increasing order
BOOST_AUTO_TEST_CASE(errorWhenZeroLociInWrongOrder) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.2 0.1 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error when negative loci locations
BOOST_AUTO_TEST_CASE(errorWhenNegativeLocusLocation) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 -0.2 0.1 0.5 0.8\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}

// Should error when locus beyond last chromosome end
BOOST_AUTO_TEST_CASE(errorWhenLocusBeyondLastChromosomeEnd) {

    std::ofstream archfile;
    archfile.open("architecture.txt");
    archfile << "3 0.1 0.33 1\n"; // no. of chromosomes and chromosome ends
    archfile << "4 0.2 0.1 0.5 1.1\n"; // no. of loci and locus locations
    archfile.close();

    std::ofstream file;
    file.open("parameters.txt");
    file << "loadarch 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);

}