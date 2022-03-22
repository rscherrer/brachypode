#ifndef BRACHYPODE_INDIVIDUAL_H
#define BRACHYPODE_INDIVIDUAL_H

// This is the header for the Individual class.

#include "random.h"
#include "parameters.h"
#include "architecture.h"
#include <vector>
#include <cassert>
#include <bitset>

class Individual {

public:

    Individual();

    void kill();
    void setDeme(const size_t&);
    void setPatch(const size_t&);
    void setX(const double&);
    void mutate(const double&, const size_t&);
    void develop(const size_t&, const double&, const double&, const double&, const double&);
    void recombine(const double&, const Individual&, const std::vector<double>&, const std::vector<double>&);

    size_t getDeme() const;
    size_t getPatch() const;
    double getX() const;
    double getY() const;
    double getZ() const;
    bool isAlive() const;
    size_t getAllele(const size_t&) const;
    size_t getAlleleSum() const;

private:

    size_t deme;
    size_t patch;
    double x;
    double y;
    double z;
    bool alive;
    std::bitset<1000> genome;

};

#endif
