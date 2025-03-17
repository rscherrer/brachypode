#ifndef BRACHYPODE_PARAMETERS_HPP
#define BRACHYPODE_PARAMETERS_HPP

// Parameter set. Contains values of the parameters of the simulation.
// All parameters have default values that can be modified by calling
// the program with a parameter file name as unique argument.

#include "random.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cassert>
#include <cstdint>

struct Parameters {

    Parameters();

    void read(const std::string&);
    void update();
    void import(std::ifstream&);
    void changeClimate(const double&);
    void write(std::ofstream&) const;
    void save() const;
    void check() const;
    size_t makeDefaultSeed();

    size_t popsize; // initial population size
    std::vector<double> pgood; // proportion of good patches in each deme
    std::vector<double> pgoodEnd; // pgood after climate change
    std::vector<double> stress; // stress level in each patch
    std::vector<double> stressEnd; // stress after climate change
    std::vector<double> capacities; // carrying capacity in each patch
    std::vector<double> capacitiesEnd; // carrying capacities after climate change
    double maxgrowth; // maximum growth rate
    double steep; // steepness of tolerance function
    double dispersal; // dispersal rate
    double mutation; // mutation rate
    size_t nchrom; // number of chromosomes
    size_t nloci; // number of loci
    double effect; // locus effect size
    double allfreq; // initial frequency of allele 1
    double tradeoff; // tradeoff between tolerance and competitiveness
    double selfing; // rate of selfing
    double recombination; // recombination rate
    size_t tend; // simulation time
    size_t tsave; // recording time
    size_t tchange; // time to initiate warming
    size_t twarming; // duration of the warming period
    size_t type; // type of trade off implementation
    size_t seed;
    bool sow;
    bool loadarch;
    bool savepars;
    bool savelog;
    bool savearch;
    bool talkative;
    bool choose;

};

#endif