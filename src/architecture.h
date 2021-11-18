#ifndef BRACHYPODE_ARCHITECTURE_H
#define BRACHYPODE_ARCHITECTURE_H

#include "param.h"
#include <vector>

class Architecture {

public:

    Architecture(const Param&);

    size_t getNLoci() const;
    size_t getNChrom() const;
    double getChromEnd(const size_t&) const;
    double getLocation(const size_t&) const;
    size_t getChromosome(const size_t&) const;
    size_t getTrait(const size_t&) const;
    double getEffect(const size_t&) const;

private:

    size_t nloci; // number of loci in the genome
    std::vector<double> chromends; // genomic location of chromosome ends
    std::vector<double> locations; // genomic location of loci
    std::vector<size_t> chromosomes; // chromosomal location of loci
    std::vector<size_t> traits; // traits encoded by loci
    std::vector<double> effects; // effect sizes of loci

};

#endif
