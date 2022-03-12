#ifndef BRACHYPODE_ARCHITECTURE_H
#define BRACHYPODE_ARCHITECTURE_H

#include "parameters.h"
#include <vector>

class Architecture {

public:

    Architecture(const Parameters&);

    void load();

    size_t getNLoci() const;
    size_t getNTol() const;
    size_t getNComp() const;
    size_t getNNeut() const;
    size_t getNChrom() const;
    double getChromEnd(const size_t&) const;
    double getLocation(const size_t&) const;
    size_t getChromosome(const size_t&) const;
    size_t getTrait(const size_t&) const;
    double getEffect(const size_t&) const;

private:

    size_t nchrom; // number of chromosomes
    size_t ntol; // number of loci coding for tolerance
    size_t ncomp; // number of loci coding for competitiveness
    size_t nneut; // number of neutral loci
    size_t nloci; // number of loci in the genome

    std::vector<double> chromends; // genomic location of chromosome ends
    std::vector<double> locations; // genomic location of loci
    std::vector<size_t> chromosomes; // chromosomal location of loci
    std::vector<size_t> traits; // traits encoded by loci
    std::vector<double> effects; // effect sizes of loci

};

#endif
