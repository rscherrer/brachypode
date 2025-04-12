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

    // Prepare content of parameter file
    std::ostringstream content;

    // Add lines
    content << "popsize 10\n";
    content << "ndemes 2\n";
    content << "pgood 0.1 0.2\n";
    content << "pgoodEnd 0.1 0.2\n";
    content << "capacities 10 20\n";
    content << "capacitiesEnd 10 20\n";
    content << "stress 0.1 0.2\n";
    content << "stressEnd 0.1 0.2\n";
    content << "maxgrowth 0.1\n";
    content << "steep 0.1\n";
    content << "dispersal 0.1\n";
    content << "mutation 0.1\n";
    content << "recombination 0.1\n";
    content << "nloci 4\n";
    content << "effect 0.1\n";
    content << "tradeoff 0.1\n";
    content << "nonlinear 0.1\n";
    content << "selfing 0.1\n";
    content << "allfreq 0.1\n";
    content << "minrealk 0.1\n";
    content << "memsave 0.1\n";
    content << "tend 10\n"; 
    content << "tsave 1\n";
    content << "twarming 10\n";
    content << "tchange 10\n";
    content << "seed 42\n";
    content << "sow 1\n";
    content << "loadarch 0\n";
    content << "savepars 1\n";
    content << "savearch 1\n";
    content << "savedat 1\n";
    content << "choose 1\n";
    content << "verbose 1\n";

    // Read the parameter file
    Parameters pars("parameters.txt");

    // Check that the parameters have been updated
    BOOST_CHECK_EQUAL(pars.popsize, 10u);
    BOOST_CHECK_EQUAL(pars.ndemes, 2u);
    BOOST_CHECK_EQUAL(pars.pgood[0u], 0.1);
    BOOST_CHECK_EQUAL(pars.pgood[1u], 0.2);
    BOOST_CHECK_EQUAL(pars.pgoodEnd[0u], 0.1);
    BOOST_CHECK_EQUAL(pars.pgoodEnd[1u], 0.2);
    BOOST_CHECK_EQUAL(pars.capacities[0u], 10.0);
    BOOST_CHECK_EQUAL(pars.capacities[1u], 20.0);
    BOOST_CHECK_EQUAL(pars.capacitiesEnd[0u], 10.0);
    BOOST_CHECK_EQUAL(pars.capacitiesEnd[1u], 20.0);
    BOOST_CHECK_EQUAL(pars.stress[0u], 0.1);
    BOOST_CHECK_EQUAL(pars.stress[1u], 0.2);
    BOOST_CHECK_EQUAL(pars.stressEnd[0u], 0.1);
    BOOST_CHECK_EQUAL(pars.stressEnd[1u], 0.2);
    BOOST_CHECK_EQUAL(pars.maxgrowth, 0.1);
    BOOST_CHECK_EQUAL(pars.steep, 0.1);
    BOOST_CHECK_EQUAL(pars.dispersal, 0.1);
    BOOST_CHECK_EQUAL(pars.mutation, 0.1);
    BOOST_CHECK_EQUAL(pars.recombination, 0.1);
    BOOST_CHECK_EQUAL(pars.nloci, 4u);
    BOOST_CHECK_EQUAL(pars.effect, 0.1);
    BOOST_CHECK_EQUAL(pars.tradeoff, 0.1);
    BOOST_CHECK_EQUAL(pars.nonlinear, 0.1);
    BOOST_CHECK_EQUAL(pars.selfing, 0.1);
    BOOST_CHECK_EQUAL(pars.allfreq, 0.1);
    BOOST_CHECK_EQUAL(pars.minrealk, 0.1);
    BOOST_CHECK_EQUAL(pars.memsave, 0.1);
    BOOST_CHECK_EQUAL(pars.tend, 10u);
    BOOST_CHECK_EQUAL(pars.tsave, 1u);
    BOOST_CHECK_EQUAL(pars.twarming, 10u);
    BOOST_CHECK_EQUAL(pars.tchange, 10u);
    BOOST_CHECK_EQUAL(pars.seed, 42u);
    BOOST_CHECK(pars.sow);
    BOOST_CHECK(!pars.loadarch);
    BOOST_CHECK(pars.savepars);
    BOOST_CHECK(pars.savearch);
    BOOST_CHECK(pars.savedat);
    BOOST_CHECK(pars.choose);
    BOOST_CHECK(pars.verbose);

    // Remove files
    std::remove("parameters.txt");

}

// TODO: Test missing file in main

// Test that parameter reading fails when invalid parameters are provided
BOOST_AUTO_TEST_CASE(readInvalidParameters)
{

    // Write a file with invalid parameters
    tst::write("parameters.txt", "popsize 10\nselfing 0.95\ninvalid 42\n");

    // Try to...
    tst::checkError([&]() {

        // Read the file
        Parameters pars("parameters.txt");

    }, "Invalid parameter: invalid in line 3 of file parameters.txt");

    // Remove files
    std::remove("parameters.txt");

}

// Test error upon invalid starting population size
BOOST_AUTO_TEST_CASE(readInvalidPopSize)
{

    // Write a file with invalid population size
    tst::write("p1.txt", "popsize 0\n");
    tst::write("p2.txt", "popsize 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter popsize must be strictly positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter popsize in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid number of demes
BOOST_AUTO_TEST_CASE(readInvalidNDemes)
{

    // Write a file with invalid number of demes
    tst::write("p1.txt", "ndemes 0\n");
    tst::write("p2.txt", "ndemes 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter ndemes must be strictly positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter ndemes in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid proportion of good patches
BOOST_AUTO_TEST_CASE(readInvalidPGood)
{

    // Write a file with invalid proportion of good patches
    tst::write("p1.txt", "ndemes 2\npgood 0.5 0.5 0.5\npgoodEnd 0.5 0.5\n");
    tst::write("p2.txt", "ndemes 2\npgood 0.5\npgoodEnd 0.5 0.5\n");
    tst::write("p3.txt", "ndemes 2\npgood 0.5 1.5\npgoodEnd 0.5 0.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter pgood in line 2 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too few values for parameter pgood in line 2 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter pgood must be between 0 and 1 in line 2 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid proportion of good patches after warming
BOOST_AUTO_TEST_CASE(readInvalidPGoodEnd)
{

    // Write a file with invalid proportion of good patches after warming
    tst::write("p1.txt", "ndemes 2\npgood 0.5 0.5\npgoodEnd 0.5 0.5 0.5\n");
    tst::write("p2.txt", "ndemes 2\npgood 0.5 0.5\npgoodEnd 0.5\n");
    tst::write("p3.txt", "ndemes 2\npgood 0.5 0.5\npgoodEnd 0.5 1.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter pgoodEnd in line 3 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too few values for parameter pgoodEnd in line 3 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter pgoodEnd must be between 0 and 1 in line 3 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid stress levels
BOOST_AUTO_TEST_CASE(readInvalidStress)
{

    // Write a file with invalid stress levels
    tst::write("p1.txt", "stress 0.5 0.5 0.5");
    tst::write("p2.txt", "stress 0.5");
    tst::write("p3.txt", "stress 0.5 -0.5");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter stress in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too few values for parameter stress in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter stress must be positive in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid stress levels after warming
BOOST_AUTO_TEST_CASE(readInvalidStressEnd)
{

    // Write a file with invalid stress levels after warming
    tst::write("p1.txt", "stressEnd 0.5 0.5 0.5");
    tst::write("p2.txt", "stressEnd 0.5");
    tst::write("p3.txt", "stressEnd 0.5 -0.5");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter stressEnd in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too few values for parameter stressEnd in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter stressEnd must be positive in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid carrying capacities
BOOST_AUTO_TEST_CASE(readInvalidCapacities)
{

    // Write a file with invalid carrying capacities
    tst::write("p1.txt", "capacities 0.5 0.5 0.5");
    tst::write("p2.txt", "capacities 0.5");
    tst::write("p3.txt", "capacities 10 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter capacities in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too few values for parameter capacities in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter capacities must be strictly positive in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid carrying capacities after warming
BOOST_AUTO_TEST_CASE(readInvalidCapacitiesEnd)
{

    // Write a file with invalid carrying capacities after warming
    tst::write("p1.txt", "capacitiesEnd 0.5 0.5 0.5");
    tst::write("p2.txt", "capacitiesEnd 0.5");
    tst::write("p3.txt", "capacitiesEnd 10 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter capacitiesEnd in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too few values for parameter capacitiesEnd in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter capacitiesEnd must be strictly positive in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid maximum growth rate
BOOST_AUTO_TEST_CASE(readInvalidMaxGrowth)
{

    // Write a file with invalid maximum growth rate
    tst::write("p1.txt", "maxgrowth 0.5 0.5");
    tst::write("p2.txt", "maxgrowth -0.5");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter maxgrowth in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter maxgrowth must be positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid steepness of the tolerance function
BOOST_AUTO_TEST_CASE(readInvalidSteep)
{

    // Write a file with invalid steepness of the tolerance function
    tst::write("p1.txt", "steep 0.5 0.5");
    tst::write("p2.txt", "steep -0.5");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter steep in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter steep must be positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid dispersal rate
BOOST_AUTO_TEST_CASE(readInvalidDispersal)
{

    // Write a file with invalid dispersal rate
    tst::write("p1.txt", "dispersal 0.5 0.5");
    tst::write("p2.txt", "dispersal 1.1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter dispersal in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter dispersal must be between 0 and 1 in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid mutation rate
BOOST_AUTO_TEST_CASE(readInvalidMutation)
{

    // Write a file with invalid mutation rate
    tst::write("p1.txt", "mutation 0.5 0.5");
    tst::write("p2.txt", "mutation 1.1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter mutation in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter mutation must be between 0 and 1 in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid number of loci
BOOST_AUTO_TEST_CASE(readInvalidNLoci)
{

    // Write a file with invalid number of loci
    tst::write("p1.txt", "nloci 10 10");
    tst::write("p2.txt", "nloci 1001");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter nloci in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter nloci must be between 1 and 1000 in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid effect size
BOOST_AUTO_TEST_CASE(readInvalidEffect)
{

    // Write a file with invalid effect size
    tst::write("p1.txt", "effect 0.5 0.5");
    tst::write("p2.txt", "effect 0.0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter effect in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter effect must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid starting allele frequency
BOOST_AUTO_TEST_CASE(readInvalidAllFreq)
{

    // Write a file with invalid starting allele frequency
    tst::write("p1.txt", "allfreq 0.5 0.5");
    tst::write("p2.txt", "allfreq 1.1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter allfreq in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter allfreq must be between 0 and 1 in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid tradeoff
BOOST_AUTO_TEST_CASE(readInvalidTradeOff)
{

    // Write a file with invalid tradeoff
    tst::write("p1.txt", "tradeoff 0.5 0.5");
    tst::write("p2.txt", "tradeoff -0.5");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter tradeoff in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter tradeoff must be positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid nonlinear effect
BOOST_AUTO_TEST_CASE(readInvalidNonlinear)
{

    // Write a file with invalid nonlinear effect
    tst::write("p1.txt", "nonlinear 0.5 0.5");
    tst::write("p2.txt", "nonlinear 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter nonlinear in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter nonlinear must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid selfing rate
BOOST_AUTO_TEST_CASE(readInvalidSelfing)
{

    // Write a file with invalid selfing rate
    tst::write("p1.txt", "selfing 0.5 0.5");
    tst::write("p2.txt", "selfing 1.1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter selfing in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter selfing must be between 0 and 1 in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid minimum realized carrying capacity
BOOST_AUTO_TEST_CASE(readInvalidMinRealK)
{

    // Write a file with invalid minimum realized carrying capacity
    tst::write("p1.txt", "minrealk 0.5 0.5");
    tst::write("p2.txt", "minrealk 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter minrealk in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter minrealk must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid saving memory
BOOST_AUTO_TEST_CASE(readInvalidMemSave)
{

    // Write a file with invalid saving memory
    tst::write("p1.txt", "memsave 0.5 0.5");
    tst::write("p2.txt", "memsave 0.00000001");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter memsave in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter memsave must be enough MB to store a double in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid end time
BOOST_AUTO_TEST_CASE(readInvalidTEnd)
{

    // Write a file with invalid end time
    tst::write("p1.txt", "tend 10 10");
    tst::write("p2.txt", "tend 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter tend in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter tend must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid save time
BOOST_AUTO_TEST_CASE(readInvalidTSave)
{

    // Write a file with invalid save time
    tst::write("p1.txt", "tsave 10 10");
    tst::write("p2.txt", "tsave 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter tsave in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter tsave must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid warming time
BOOST_AUTO_TEST_CASE(readInvalidTChange)
{

    // Write a file with invalid warming time
    tst::write("p1.txt", "tchange 10 10");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter tchange in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test error upon invalid warming duration
BOOST_AUTO_TEST_CASE(readInvalidTWarming)
{

    // Write a file with invalid warming duration
    tst::write("p1.txt", "twarming 10 10");
    tst::write("p2.txt", "twarming 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter twarming in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter twarming must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    
}

// Test error upon invalid seed
BOOST_AUTO_TEST_CASE(readInvalidSeed)
{

    // Write a file with invalid seed
    tst::write("p1.txt", "seed 10 10");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter seed in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid sowing flag
BOOST_AUTO_TEST_CASE(readInvalidSow)
{

    // Write a file with invalid sowing flag
    tst::write("p1.txt", "sow 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter sow in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid architecture loading flag
BOOST_AUTO_TEST_CASE(readInvalidLoadArch)
{

    // Write a file with invalid architecture loading flag
    tst::write("p1.txt", "loadarch 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter loadarch in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid parameter saving flag
BOOST_AUTO_TEST_CASE(readInvalidSavePars)
{

    // Write a file with invalid parameter saving flag
    tst::write("p1.txt", "savepars 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter savepars in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid architecture saving flag
BOOST_AUTO_TEST_CASE(readInvalidSaveArch)
{

    // Write a file with invalid architecture saving flag
    tst::write("p1.txt", "savearch 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter savearch in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid data saving flag
BOOST_AUTO_TEST_CASE(readInvalidSaveDat)
{

    // Write a file with invalid data saving flag
    tst::write("p1.txt", "savedat 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter savedat in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid choice flag
BOOST_AUTO_TEST_CASE(readInvalidChoose)
{

    // Write a file with invalid choice flag
    tst::write("p1.txt", "choose 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter choose in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid verbosity flag
BOOST_AUTO_TEST_CASE(readInvalidVerbose)
{

    // Write a file with invalid verbosity flag
    tst::write("p1.txt", "verbose 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter verbose in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
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