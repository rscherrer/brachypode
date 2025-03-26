#ifndef BRACHYPODE_ARCHITECTURE_HPP
#define BRACHYPODE_ARCHITECTURE_HPP

// This is the header for the Architecture class. An Architecture object
// contains all the details of the genotype-phenotype map that take too much
// space to be placed within a Parameters object (e.g. if there are many loci).
// The parameters of the genetic architecture (e.g. locus locations)
// can be generated randomly (this is the default upon construction) or loaded
// from an architecture file.

#include "parameters.hpp"
#include <vector>

struct ArchPars {

    size_t nchrom;  // number of chromsomes
    size_t nloci;   // number of loci
    double effect;  // locus effect size

};
 
class Architecture {

public:

    // Constructor
    Architecture(const ArchPars&, const std::string& = "");

    // Functions
    void save(const std::string&) const;

private:

    // Hyperparameters
    size_t nchrom;
    size_t nloci;
    
    // Parameters
    std::vector<double> chromends; // ends of chromosomes
    std::vector<double> locations; // gene locations
    std::vector<double> effects;   // effect sizes

    // Maximum trait value
    double tolmax;

    // Private setters
    void read(const std::string&);

};

#endif