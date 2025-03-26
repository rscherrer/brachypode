// This script contains all the functions of the Individual class.

#include "individual.hpp"
#include <cmath>

// Constructor
Individual::Individual(const double &freq, const std::shared_ptr<Architecture> &arch) :
    tolerance(0.0),    
    deme(0u),
    patch(1u),
    nseeds(0u),
    alive(true),
    alleles(std::bitset<1000>()),
    genetics(arch)
{

    // freq: frequency of allele 1
    // arch: a genetic architecture

    // Check that the genome only has zeros
    assert(alleles.count() == 0u);

    // Prepare a mutation sampler
    auto isMutation = rnd::bernoulli(freq);

    // For each locus...
    for (size_t i = 0u; i < genetics->nloci; ++i) {

        // If there is a mutation...
        if (isMutation(rnd::rng)) {
         
            // Flip the allele
            flip(i);
            
        }
    }
}

// Function to kill an individual
void Individual::kill() { alive = false; }

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
    tolerance += genetics->effects[i] * (alleles.test(i) * 2.0 - 1.0);

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
        for (size_t i = 0u; i < genetics->nloci; ++i) flip(i);

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
        if (i >= genetics->nloci) break;

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
    double position = genetics->locations[0u];
    double chromend = genetics->chromends[0u];

    // Sample the starting haplotype
    size_t hap = getHaplotype(rnd::rng);

    // While we progress through loci...
    while (locus < genetics->nloci) {

        // What is the next thing coming up?
        size_t next = static_cast<size_t>(crossover);

        // Is it a crossover, a chromosome end, or a locus?
        if (crossover < chromend && crossover < position) next = 0u;
        else if (chromend < crossover && chromend < position) next = 1u;
        else next = 2u;

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
            if (chrom < genetics->nchrom) chromend = genetics->chromends[chrom];

            // Make sure we are not beyond the last chromosome
            assert(chrom < genetics->nchrom);

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
            if (locus < genetics->nloci) position = genetics->locations[locus];

            break;
        }
    }

    // Safety checks
    assert(locus == genetics->nloci);
    assert(chrom == genetics->nchrom - 1u);

}

// Getters
size_t Individual::getDeme() const { return deme; }
size_t Individual::getPatch() const { return patch; }
size_t Individual::getNSeeds() const { return nseeds; }
double Individual::getTolerance() const { return tolerance; }
bool Individual::isAlive() const { return alive; }
size_t Individual::getAllele(const size_t &l) const { return alleles.test(l); }
size_t Individual::countAlleles() const { return alleles.count(); }
