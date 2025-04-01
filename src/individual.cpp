// This script contains all the functions of the Individual class.

#include "individual.hpp"

// Constructor
Individual::Individual(const double &freq, const std::shared_ptr<Architecture> &arch) :
    tolerance(0.0),    
    deme(0u),
    patch(1u),
    nseeds(0u),
    alleles(std::bitset<1000>()),
    architecture(arch)
{

    // freq: frequency of allele 1
    // arch: a genetic architecture

    // Check that the genome only has zeros
    assert(alleles.count() == 0u);

    // Prepare a mutation sampler
    auto isMutation = rnd::bernoulli(freq);

    // For each locus...
    for (size_t i = 0u; i < architecture->nloci; ++i) {

        // If there is a mutation...
        if (isMutation(rnd::rng)) {
         
            // Flip the allele
            flip(i);
            
        }
    }
}

// Resetters
void Individual::setDeme(const size_t &d) { deme = d; }
void Individual::setPatch(const size_t &p) { patch = p; }
void Individual::setNSeeds(const size_t &n) { nseeds = n; }
void Individual::setTolerance(const double &x) { tolerance = x; }

// Function to update trait value when an allele has just flipped
void Individual::flip(const size_t &i) {

    // i: index of the allele to flip

    // Flip the allele
    alleles.flip(i);

    // Update trait value
    tolerance += architecture->effects[i] * (alleles.test(i) * 2.0 - 1.0);

    // Note: this translates the new allele into plus or minus one.

}

// Function to mutate the genome
void Individual::mutate(const double &mu) {

    // mu: mutation rate

    // No mutation if the rate is zero
    if (mu == 0.0) return;

    // If the rate is one...
    if (mu == 1.0) { 
        
        // Flip each locus
        for (size_t i = 0u; i < architecture->nloci; ++i) flip(i);

        // Exit
        return; 
    
    }

    // Prepare a next mutation sampler
    auto getNextMutant = rnd::iotagap(mu);

    // Reset the sampler
    getNextMutant.reset(0u);

    // For as long as it takes...
    for (;;) {

        // Sample the next mutation
        const size_t i = getNextMutant(rnd::rng);

        // Stop if we are beyond the end of the genome
        if (i >= architecture->nloci) break;

        // Flip the sampled position
        flip(i);

    }
}

// TODO: Remove chromosomes and free recombination
// TODO: Check whether we need the iota gap sampler

// Function to recombine genome with a pollen donor
void Individual::recombine(const double &rho, const Individual &pollen) {

    // rho: recombination rate
    // pollen: pollen donor individual

    // Exit if no recombination
    if (rho == 0.0) return;

    // Check
    assert(rho > 0.0);

    // Initialization
    size_t locus = 0u;

    // Crossovers are sampled from an exponential distribution
    auto getNextCrossover = rnd::exponential(rho);

    /// Sample the first crossover point
    double crossover = getNextCrossover(rnd::rng);

    // Initialize the current position
    double position = architecture->locations[0u];

    // Sample the starting haplotype
    size_t hap = rnd::bernoulli(0.5)(rnd::rng);

    // While we progress through loci...
    while (locus < architecture->nloci) {

        // If the next thing coming up is a crossover point...
        if (crossover < position) {

            // Switch haplotype
            hap = hap ? 0u : 1u;

            // Update current crossover location
            crossover += getNextCrossover(rnd::rng);

        } else {

            // Note: Otherwise, it is a locus

            // If we are on the opposite (i.e. pollen) haplotype
            if (hap) {

                // If the pollen has a different allele...
                if (alleles.test(locus) != pollen.getAllele(locus)) {

                    // Flip the local allele
                    flip(locus);
                    
                }
            }

            // Move on to the next locus
            ++locus;

            // Update current position
            if (locus < architecture->nloci) position = architecture->locations[locus];

        }
    }

    // Safety checks
    assert(locus == architecture->nloci);

}

// Function to check validity of members
void Individual::check(const size_t &n) {

    // n: number of demes

    // Check
    assert(patch < 2u);
    assert(deme < n);
    assert(tolerance >= 0.0);

}