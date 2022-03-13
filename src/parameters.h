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
    std::vector<double> maxgrowths; // maximum growth rate in each patch
    std::vector<double> xopts; // optimal trait value in each patch
    std::vector<double> xwidths; // niche width in each patch
    std::vector<double> capacities; // carrying capacity in each patch
    double dispersal; // dispersal rate
    double mutation; // mutation rate
    size_t nchrom; // number of chromosomes
    size_t nloci; // number of loci
    double effect; // locus effect size
    double selfing; // rate of selfing
    double recombination; // recombination rate

    size_t tend; // simulation time

    size_t seed;

    bool loadarch;
    bool savepars;
    bool savelog;
    bool savearch;
    bool talkative;

};

#endif
