#ifndef BRACHYPODE_INDIVIDUAL_H
#define BRACHYPODE_INDIVIDUAL_H

#include "Random.h"
#include <vector>
#include <cassert>

class Individual {

    friend class Population;

public:

    Individual();

    double getTraitValue() const;

private:

    double x;

};

#endif
