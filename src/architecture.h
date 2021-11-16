#ifndef BRACHYPODE_ARCHITECTURE_H
#define BRACHYPODE_ARCHITECTURE_H

#include "param.h"
#include "random.h"

class Architecture {

public:

    Architecture(const Param&);

    size_t getNLoci() const;
    size_t getNChrom() const;
    double getChromend(const size_t&) const;
    double getLocation(const size_t&) const;
    size_t getChromosome(const size_t&) const;
    size_t getTrait(const size_t&) const;
    //double getEffect(const size_t&) const;

private:

    size_t nloci; // number of loci
    std::vector<double> chromends; // chromosome endpoints
    std::vector<double> locations; // locus locations along the genome
    std::vector<size_t> chromosomes; // chromosomal location
    std::vector<size_t> traits; // encoded traits by each locus
    //std::vector<double> effects; // effect sizes of the loci

};

#endif
