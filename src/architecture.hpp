#ifndef BRACHYPODE_ARCHITECTURE_HPP
#define BRACHYPODE_ARCHITECTURE_HPP

// This is the header for the Architecture class. An Architecture object
// contains all the details of the genotype-phenotype map that take too much
// space to be placed within a Parameters object (e.g. if there are many loci).
// The parameters of the genetic architecture (e.g. locus locations)
// can be generated randomly (this is the default upon construction) or loaded
// from an architecture file.

#include "parameters.hpp"
#include "random.hpp"
 
struct Architecture {

    // Constructor
    Architecture(const Parameters&, const std::string& = "");

    // Functions
    void check() const;
    void make();
    void read(const std::string&);
    void save(const std::string&) const;

    // Hyperparameters
    size_t nloci;
    double tolmax;
    
    // Parameters
    std::vector<double> locations; // gene locations
    std::vector<double> effects;   // effect sizes

};

#endif