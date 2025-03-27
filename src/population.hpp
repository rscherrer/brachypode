#ifndef BRACHYPODE_POPULATION_HPP
#define BRACHYPODE_POPULATION_HPP

// This is the header for the Population class, which encapsulates
// a vector of Individuals and makes them go through each generation
// of the simulation.

#include "printer.hpp"
#include "parameters.hpp"
#include "architecture.hpp"
#include "individual.hpp"

class Population {

public:

    // Constructor
    Population(const Parameters&, const Architecture&);

    // Setters
    void show() const;
    void update();
    void cycle(Printer&);
    void moveon();

    // Main getters
    size_t size() const;
    bool keepon() const;
    bool extinct() const;
    size_t deme(const size_t&) const;

    // Other getters
    int getTime() const;
    double getPGood(const size_t&) const;
    double getStress(const size_t&) const;
    double getCapacity(const size_t&) const;

private:

    // The individuals
    std::unique_ptr<std::vector<Individual> > individuals;
    std::unique_ptr<std::vector<Individual> > newborns;

    // Parameters
    size_t ndemes;                     // number of demes
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
    double allfreq;                    // initial frequency of allele 1
    double tradeoff;                   // tradeoff between tolerance and fertility
    double dvalue;                     // useful exponent for nonlinear tradeoff
    double selfing;                    // rate of selfing
    double recombination;              // recombination rate
    double tolmax;                     // maximum stress tolerance possible 
    size_t tend;                       // simulation time
    size_t tsave;                      // recording time
    size_t tchange;                    // time to initiate warming
    size_t twarming;                   // duration of the warming period
    bool linear;                       // linear or nonlinear tradeoff implementation?
    bool sow;                          // sow individuals at random

    // Variables
    int time;
    size_t popsize;
    std::vector<size_t> demesizes;
    std::vector<size_t> patchsizes;
    std::vector<double> meantol;

    // Private setters
    void reset();
    void gather(const size_t&, const size_t&, const double&);
    void shuffle();

};

#endif