#ifndef BRACHYPODE_ARCHITECTURE_HPP
#define BRACHYPODE_ARCHITECTURE_HPP

// This is the header for the Architecture structure. An Architecture object
// contains all the details of the genotype-phenotype map that take too much
// space to be placed within a Parameters object (e.g. if there are many loci).
// The parameters of the genetic architecture (e.g. locus locations)
// can be generated randomly (this is the default upon construction) or loaded
// from an architecture file ("architecture.txt").

#include "parameters.hpp"
#include <vector>

struct Architecture {

    Architecture(const size_t&, const size_t&, const double&);

    void load();
    void save(const Parameters&) const;

    std::vector<double> chromends; // ends of chromosomes
    std::vector<double> locations; // gene locations
    std::vector<double> effects; // effect sizes

};

#endif