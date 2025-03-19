#ifndef BRACHYPODE_PARAMETERS_HPP
#define BRACHYPODE_PARAMETERS_HPP

// This is the header for the Parameters class. The Parameters class contains values 
// of the parameters of the simulation. All parameters have default values that can be 
// modified by reading in a parameter text file with user-defined values.

#include <fstream>
#include <iostream>
#include <chrono>
#include <cassert>
#include <cstdint>

struct Parameters {

    // Constructor
    Parameters();

    // Functions
    void read(const std::string&);
    void update(const int&);
    void save() const;
    void check() const;

    // Parameters
    size_t popsize;                    // initial population size
    std::vector<double> pgood;         // proportion of good patches in each deme
    std::vector<double> pgoodEnd;      // pgood after climate change
    std::vector<double> stress;        // stress level in each patch
    std::vector<double> stressEnd;     // stress after climate change
    std::vector<double> capacities;    // carrying capacity in each patch
    std::vector<double> capacitiesEnd; // carrying capacities after climate change
    double maxgrowth;                  // maximum growth rate
    double steep;                      // steepness of tolerance function
    double dispersal;                  // dispersal rate
    double mutation;                   // mutation rate
    size_t nchrom;                     // number of chromosomes
    size_t nloci;                      // number of loci
    double effect;                     // locus effect size
    double allfreq;                    // initial frequency of allele 1
    double tradeoff;                   // tradeoff between tolerance and fertility
    double selfing;                    // rate of selfing
    double recombination;              // recombination rate
    size_t tend;                       // simulation time
    size_t tsave;                      // recording time
    size_t tchange;                    // time to initiate warming
    size_t twarming;                   // duration of the warming period
    size_t type;                       // type of trade off implementation
    size_t seed;                       // seed for random number generator
    bool sow;                          // sow individuals at random
    bool loadarch;                     // load genetic architecture from file
    bool savepars;                     // save parameters to file
    bool savelog;                      // redirect screen output to log file
    bool savearch;                     // save genetic architecture to file
    bool savedat;                      // save output into data files
    bool choose;                       // choose which output(s) to save
    bool verbose;                      // print progress to screen

};

#endif