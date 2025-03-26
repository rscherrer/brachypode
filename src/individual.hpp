#ifndef BRACHYPODE_INDIVIDUAL_HPP
#define BRACHYPODE_INDIVIDUAL_HPP

// This is the header for the Individual class. It contains all the basic elements an 
// individual organism needs in the simulation such as trait values, genetic information,
// and location in the landscape.

#include "random.hpp"
#include "parameters.hpp"
#include "architecture.hpp"
#include <bitset>
#include <cmath>

class Individual {

public:

    // Constructor
    Individual(const double&, const std::shared_ptr<Architecture>&);

    // Setters
    void mutate(const double&);
    void recombine(const double&, const Individual&);

    // Resetters
    void setDeme(const size_t&);
    void setPatch(const size_t&);
    void setNSeeds(const size_t&);
    void setTolerance(const double&);

    // Getters
    size_t getDeme() const;
    size_t getPatch() const;
    size_t getNSeeds() const;
    size_t getAllele(const size_t&) const;
    size_t countAlleles() const;
    double getTolerance() const;

private:

    // Attributes
    double tolerance;
    size_t deme;
    size_t patch;
    size_t nseeds;

    // Alleles as a bitset
    std::bitset<1000> alleles;

    // Pointer to shared genetic parameters
    std::shared_ptr<Architecture> architecture;

    // Private setter
    void flip(const size_t&);

};

#endif
