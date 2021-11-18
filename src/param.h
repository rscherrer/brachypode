#ifndef BRACHYPODE_PARAM_H
#define BRACHYPODE_PARAM_H

#include "random.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cassert>
#include <cstdint>

// Parameter set. Contains values of the parameters of the simulation.
// All parameters have default values that can be modified by calling
// the program with a parameter file name as unique argument.

struct Param {

    Param();

    void read(const std::string&);
    void update();
    void import(std::ifstream&);

    void write(std::ofstream&) const;
    void save() const;
    void check() const;

    size_t makeDefaultSeed();

    size_t popsize; // initial population size
    size_t nfacil; // number of facilitated patches
    std::vector<double> area; // areas of the two kinds of patches
    std::vector<size_t> capacity; // carrying capacities in and out
    std::vector<double> stress; // stress levels in and out
    double competition; // degree of competition in the facilitated patches
    double maxgrowth; // maximum growth rate
    double steepness; // speed of decay at tolerance threshold
    double costcomp; // cost of competitiveness
    double costtol; // cost of tolerance
    double tradeoff; // tradeoff between the two
    size_t nchrom; // number of chromosomes
    size_t ncomp; // number of loci coding for competitiveness
    size_t ntol; // number of loci coding for tolerance
    size_t nneut; // number of neutral loci
    double allfreq; // initial allele frequency
    double mutation; // mutation rate
    double effect; // effect size of a locus
    size_t seed;

    bool savepars;
    bool loadlandscape;
    bool loadarch;

};

#endif
