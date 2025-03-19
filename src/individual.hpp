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
    Individual(const double&, const std::vector<double>&);

    // Setters
    void kill();
    void setDeme(const size_t&);
    void setPatch(const size_t&);
    void setX(const double&);
    void mutate(const double&, const size_t&);
    void develop(const std::vector<double>&);
    void recombine(const double&, const Individual&, const std::vector<double>&, const std::vector<double>&);

    // Getters
    size_t getDeme() const;
    size_t getPatch() const;
    double getX() const;
    bool isAlive() const;
    size_t getAllele(const size_t&) const;
    size_t getAlleleSum() const;

private:

    // Member variables
    size_t deme;
    size_t patch;
    double x;
    bool alive;
    std::bitset<1000> genome;

};

#endif
