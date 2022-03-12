#ifndef BRACHYPODE_ARCHITECTURE_H
#define BRACHYPODE_ARCHITECTURE_H

#include "parameters.h"
#include <vector>

struct Architecture {

    Architecture(const size_t&, const size_t&);

    void load();
    void save(const Parameters&) const;

    std::vector<double> chromends; // ends of chromosomes
    std::vector<double> locations; // gene locations

};

#endif
