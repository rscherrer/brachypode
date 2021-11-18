#ifndef BRACHYPODE_LANDSCAPE_H
#define BRACHYPODE_LANDSCAPE_H

#include "param.h"
#include <vector>

class Landscape {

    friend class Population;

public:

    Landscape(const Param&);

    void load();
    void setArea(const size_t&, const double&);
    void setCapacity(const size_t&, const size_t&);
    void setHabitat(const size_t&, const size_t&);
    void setStress(const size_t&, const double&);
    void setCompetition(const size_t&, const double&);

    size_t getNPatches() const;

    double getArea(const size_t&) const;
    size_t getCapacity(const size_t&) const;
    size_t getHabitat(const size_t&) const;
    double getStress(const size_t&) const;
    double getCompetition(const size_t&) const;

private:

    size_t npatches;
    std::vector<double> areas;
    std::vector<size_t> capacities;
    std::vector<size_t> habitats;
    std::vector<double> stresses;
    std::vector<double> competitions;

};

#endif
