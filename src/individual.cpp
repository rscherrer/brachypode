// This script contains all the functions of the Individual class.

#include "individual.hpp"
#include <cmath>

// Constructor
Individual::Individual(const double &p, const std::vector<double> &effects) :
    deme(0u),
    patch(1u),
    x(0.0),
    alive(true),
    genome(std::bitset<1000>())
{

    // p: frequency of allele 1
    // effects: effect sizes of the loci

    // Check that the genome only has zeros
    assert(genome.count() == 0u);

    // Prepare a mutation sampler
    auto isMutation = rnd::bernoulli(p);

    // Sample mutations across the genome
    for (size_t i = 0u; i < effects.size(); ++i)
        if (isMutation(rnd::rng)) genome.set(i);

    // Compute phenotypes
    develop(effects);

}

// Simple setters
void Individual::kill() { alive = false; }
void Individual::setDeme(const size_t &d) { deme = d; }
void Individual::setPatch(const size_t &p) { patch = p; }
void Individual::setX(const double &val) { x = val; }

// Function to mutate the genome
void Individual::mutate(const double &mu, const size_t &n) {

    // mu: mutation rate
    // n: number of loci

    // No mutation if the rate is zero
    if (mu == 0.0) return;

    // All mutations if the rate is one
    if (mu == 1.0) { for (size_t i = 0u; i < n; ++i) genome.flip(i); return; }

    // Prepare a next mutation sampler
    auto getNextMutant = rnd::iotagap(mu);

    // Reset the sampler
    getNextMutant.reset(0u);

    // For as long as it takes...
    for (;;) {

        // Sample the next mutation
        const size_t mut = getNextMutant(rnd::rng);

        // Stop if we are beyond the end of the genome
        if (mut >= n) break;

        // Mutate the sampled position
        genome.flip(mut);

    }
}

// Function to (re-)compute the phenotype based on the genome
void Individual::develop(const std::vector<double> &effects) {

    // effects: effect sizes of the loci

    // Initialize the phenotype
    x = 0.0;
    
    // Sum the effects of the alleles
    for (size_t l = 0u; l < effects.size(); ++l)
        x += genome.test(l) * effects[l];

}

// Function to recombine genome with a pollen donor
void Individual::recombine(
    const double &rho, 
    const Individual &pollen,
    const std::vector<double> &chromends, 
    const std::vector<double> &locations
) {

    // rho: recombination rate
    // pollen: pollen donor individual
    // chromends: chromosome ends
    // locations: locus locations

    // Exit if no recombination
    if (rho == 0.0) return;

    // Initialization
    size_t locus = 0u;
    size_t chrom = 0u;

    // Count the number of loci and chromosomes
    const size_t nloci = locations.size();
    const size_t nchrom = chromends.size();

    // Haplotypes have equal chances to be transmitted
    auto getHaplotype = rnd::bernoulli(0.5);

    // Crossovers are sampled from an exponential distribution
    auto getNextCrossover = rnd::exponential(rho);

    // Initialize the first crossover point beyond the end of the genome
    double crossover = 1.1;

    // Sample the first crossover point
    crossover = getNextCrossover(rnd::rng);

    // Initialize the current position and chromosome end
    double position = locations[0u];
    double chromend = chromends[0u];

    // Sample the starting haplotype
    size_t hap = getHaplotype(rnd::rng);

    // While we progress through loci...
    while (locus < nloci) {

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
            if (chrom < nchrom) chromend = chromends[chrom];

            // Make sure we are not beyond the last chromosome
            assert(chrom < nchrom);

            break;

        // Upon gene...
        default:

            // If we are to read the pollen haplotype...
            if (hap) {

                // Copy the pollen allele
                if (pollen.getAllele(locus)) genome.set(locus);
                else genome.reset(locus);

            }

            // Move on to the next locus
            ++locus;

            // Update current position
            if (locus < nloci) position = locations[locus];

            break;
        }
    }

    // Safety checks
    assert(locus == nloci);
    assert(chrom == nchrom - 1u);

}

// Simple getters
size_t Individual::getDeme() const { return deme; }
size_t Individual::getPatch() const { return patch; }
double Individual::getX() const { return x; }
bool Individual::isAlive() const { return alive; }
size_t Individual::getAllele(const size_t &l) const { return genome.test(l); }
size_t Individual::getAlleleSum() const { return genome.count(); }
