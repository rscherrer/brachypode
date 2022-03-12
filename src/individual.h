#ifndef BRACHYPODE_INDIVIDUAL_H
#define BRACHYPODE_INDIVIDUAL_H

#include "random.h"
#include "parameters.h"
#include "landscape.h"
#include "architecture.h"
#include <vector>
#include <cassert>
#include <bitset>

class Individual {

public:

    Individual();

    void mutateGenome(const double&, const size_t&);
    void inherit(const Individual&, const Parameters&, const Architecture&, const bool&);
    void develop(const Architecture&);
    void setPatch(const size_t&);
    void setTolerance(const double&);
    void setCompetitiveness(const double&);
    void setFitness(const Parameters&, const Landscape&, const double&);
    void kill();

    size_t getPatch() const;
    double getTolerance() const;
    double getCompetitiveness() const;
    double getNeutral() const;
    double getFitness() const;
    bool isAlive() const;
    size_t getNOneAlleles() const;
    bool readAllele(const size_t&) const;

private:

    std::bitset<1000> genome;
    size_t patch;
    double tolerance;
    double competitiveness;
    double neutral;
    double fitness;
    bool alive;

};

#endif
