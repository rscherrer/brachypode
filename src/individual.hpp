#ifndef BRACHYPODE_INDIVIDUAL_HPP
#define BRACHYPODE_INDIVIDUAL_HPP

// This is the header for the Individual class. It contains all the basic elements an 
// individual organism needs in the simulation such as trait values, genetic information,
// and location in the landscape.

#include "random.hpp"
#include "parameters.hpp"
#include "architecture.hpp"
#include <vector>
#include <cassert>
#include <bitset>

class Individual {

public:

    // Constructor
    Individual(const double&, const std::shared_ptr<Architecture>&);

    // Setters
    void kill();
    void mutate(const double&);
    void recombine(const double&, const Individual&);

    // Resetters
    void setDeme(const size_t&);
    void setPatch(const size_t&);
    void setTolerance(const double&);

    // Getters
    size_t getDeme() const;
    size_t getPatch() const;
    size_t getAllele(const size_t&) const;
    size_t countAlleles() const;
    double getTolerance() const;
    bool isAlive() const;

private:

    // Attributes
    double tolerance;
    size_t deme;
    size_t patch;

    // Is the individual alive?
    bool alive;

    // Alleles as a bitset
    std::bitset<1000> alleles;

    // Pointer to a genetic architecture
    std::shared_ptr<Architecture> genetics;

    // Private setter
    void flip(const size_t&);

};

#endif
