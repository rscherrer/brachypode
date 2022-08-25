#ifndef BRACHYPODE_PARAMETERS_H
#define BRACHYPODE_PARAMETERS_H

// Parameter set. Contains values of the parameters of the simulation.
// All parameters have default values that can be modified by calling
// the program with a parameter file name as unique argument.

#include "random.h"
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
    void write(std::ofstream&) const;
    void save() const;
    void check() const;
    size_t makeDefaultSeed();

    size_t popsize; // initial population size
    std::vector<double> pgood; // proportion of good patches in each deme
    std::vector<double> stress; // stress level in each patch
    std::vector<double> capacities; // carrying capacity in each patch
    double maxgrowth; // maximum growth rate
    double steep; // steepness of tolerance function
    double shortrange; // short-range dispersal rate
    double longrange; // long-range dispersal rate
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
