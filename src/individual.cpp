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

// Function to recombine genome with a pollen donor
void Individual::recombine(const double &rho, const Individual &pollen) {

    // rho: recombination rate
    // pollen: pollen donor individual

    // Exit if no recombination
    if (rho == 0.0) return;

    // Initialization
    size_t locus = 0u;
    size_t chrom = 0u;

    // Haplotypes have equal chances to be transmitted
    auto getHaplotype = rnd::bernoulli(0.5);

    // Crossovers are sampled from an exponential distribution
    auto getNextCrossover = rnd::exponential(rho);

    // Initialize the first crossover point beyond the end of the genome
    double crossover = 1.1;

    // Sample the first crossover point
    crossover = getNextCrossover(rnd::rng);

    // Initialize the current position and chromosome end
    double position = architecture->locations[0u];
    double chromend = architecture->chromends[0u];

    // Sample the starting haplotype
    size_t hap = getHaplotype(rnd::rng);

    // While we progress through loci...
    while (locus < architecture->nloci) {

        // What is the next thing coming up?
        size_t next = static_cast<size_t>(crossover);

        // Is it a crossover, a chromosome end, or a locus?
        if (crossover < chromend && crossover < position) next = 0u;
        else if (chromend < crossover && chromend < position) next = 1u;
        else next = 2u;

        // Note: With this algorithm a locus sitting exactly on a chromosome
        // end will be considered first and hence taken as part of the previous
        // chromosome.

        // Depending on the case...
        switch (next) {

        // Upon crossover point...
        case 0u:

            // Switch haplotype
            hap = hap ? 0u : 1u;

            // Update current crossover location
            crossover += getNextCrossover(rnd::rng);

            break;

        // Upon free recombination point...
        case 1u:

            // Switch to random haplotype
            hap = getHaplotype(rnd::rng);

            // Move on to the next chromosome
            ++chrom;

            // Update chromosome end
            if (chrom < architecture->nchrom) chromend = architecture->chromends[chrom];

            // Make sure we are not beyond the last chromosome
            assert(chrom < architecture->nchrom);

            break;

        // Upon gene...
        default:

            // If we are to read the pollen haplotype...
            if (hap) {

                // If the pollen has a different allele...
                if (alleles.test(locus) != pollen.getAllele(locus)) {

                    // Flip the allele
                    flip(locus);
                    
                }
            }

            // Move on to the next locus
            ++locus;

            // Update current position
            if (locus < architecture->nloci) position = architecture->locations[locus];

            break;
        }
    }

    // Safety checks
    assert(locus == architecture->nloci);
    assert(chrom == architecture->nchrom - 1u);

}

// Function to check validity of members
void Individual::check(const size_t &n) {

    // n: number of demes

    // Check
    assert(patch < 2u);
    assert(deme < n);
    assert(tolerance >= 0.0);

}