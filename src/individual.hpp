#ifndef BRACHYPODE_INDIVIDUAL_HPP
#define BRACHYPODE_INDIVIDUAL_HPP

// This is the header for the Individual class.

#include "random.hpp"
#include "parameters.hpp"
#include "architecture.hpp"
#include <vector>
#include <cassert>
#include <bitset>

class Individual {

public:

    Individual(const double&, const size_t&, const std::vector<double>&);

    void kill();
    void setDeme(const size_t&);
    void setPatch(const size_t&);
    void setX(const double&);
    void mutate(const double&, const size_t&);
    void develop(const std::vector<double>&);
    void recombine(const double&, const Individual&, const std::vector<double>&, const std::vector<double>&);

    size_t getDeme() const;
    size_t getPatch() const;
    double getX() const;
    bool isAlive() const;
    size_t getAllele(const size_t&) const;
    size_t getAlleleSum() const;

private:

    size_t deme;
    size_t patch;
    double x;
    bool alive;
    std::bitset<1000> genome;

};

#endif
