#ifndef BRACHYPODE_INDIVIDUAL_H
#define BRACHYPODE_INDIVIDUAL_H

#include "random.h"
#include "param.h"
#include "landscape.h"
#include <vector>
#include "architecture.h"
#include <cassert>
#include <bitset>

class Individual {

public:

    Individual();

    void mutateGenome(const double&, const size_t&);
    void develop(const Architecture&);
    void setPatch(const size_t&);
    void setTolerance(const double&);
    void setCompetitiveness(const double&);
    void setFitness(const Param&, const Landscape&, const double&);
    void kill();

    size_t getPatch() const;
    double getTolerance() const;
    double getCompetitiveness() const;
    double getFitness() const;
    bool isAlive() const;
    size_t getNOneAlleles() const;

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
