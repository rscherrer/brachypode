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

    // Simple getters
    size_t getDeme() const { return deme; };
    size_t getPatch() const { return patch; };
    size_t getNSeeds() const { return nseeds; };
    size_t getAllele(const size_t &i) const { assert(i < alleles.size()); return alleles.test(i); }
    size_t countAlleles() const { return alleles.count(); };
    double getTolerance() const { return tolerance; };

    // Checker
    void check(const size_t&);

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

    // Private setters
    void flip(const size_t&);
    void flipall();

    // Mutation samplers
    void mutateBernoulli(const double&);
    void mutateShuffle(const double&);
    void mutateGeometric(const double&);
    void mutateBinomial(const double&);
    
};

#endif
