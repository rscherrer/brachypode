// This script contains all the functions of the Individual class

#include "individual.hpp"
#include "utilities.hpp"
#include <cmath>

Individual::Individual(const double &p, const size_t &n, const std::vector<double> &effects) :
    deme(0u),
    patch(1u),
    x(0.0),
    alive(true),
    genome(std::bitset<1000>())
{

    // Throw mutations around
    assert(genome.count() == 0u);
    auto ismutation = rnd::bernoulli(p);
    for (size_t i = 0u; i < n; ++i)
        if (ismutation(rnd::rng)) genome.set(i);

    // Compute phenotypes
    develop(effects);

}

void Individual::kill() { alive = false; }
void Individual::setDeme(const size_t &d) { deme = d; }
void Individual::setPatch(const size_t &p) { patch = p; }
void Individual::setX(const double &val) { x = val; }

void Individual::mutate(const double &mu, const size_t &n) {

    if (mu == 0.0) return;
    if (mu == 1.0) { for (size_t i = 0u; i < n; ++i) genome.flip(i); return; }

    // Mutations are sampled from a geometric distribution
    auto getnextmutant = rnd::iotagap(mu);
    getnextmutant.reset(0u);
    for (;;) {
        const size_t mut = getnextmutant(rnd::rng);
        if (mut >= n) break;
        genome.flip(mut);
    }

}

void Individual::develop(const std::vector<double> &effects) {

    x = 0.0;
    
    for (size_t l = 0u; l < effects.size(); ++l)
        x += genome.test(l) * effects[l];

}

void Individual::recombine(
    const double &rho, const Individual &pollen,
    const std::vector<double> &chromends, const std::vector<double> &locations
) {

    if (rho == 0.0) return;

    size_t locus = 0u;
    size_t chrom = 0u;

    const size_t nloci = locations.size();
    const size_t nchrom = chromends.size();

    // Haplotypes have equal chances to be transmitted
    auto gethaplotype = rnd::bernoulli(0.5);

    // Crossovers are sampled from an exponential distribution
    auto nextcrossover = rnd::exponential(rho);

    double crossover = 1.1; // beyond the end of the genome
    crossover = nextcrossover(rnd::rng);

    double position = locations[0u];
    double chromend = chromends[0u];

    // Starting haplotype
    size_t hap = gethaplotype(rnd::rng);

    // While we progress through loci...
    while (locus < nloci) {

        // What is the next thing coming up?
        size_t next = static_cast<size_t>(crossover);
        if (crossover < chromend && crossover < position)
            next = 0u; // is it a crossover point?
        else if (chromend < crossover && chromend < position)
            next = 1u; // is it a chromosome end?
        else
            next = 2u; // or is it a locus?

        // Depending on the case...
        switch (next) {

        // Upon crossover point, switch haplotype
        case 0u:
            hap = hap ? 0u : 1u;
            crossover += nextcrossover(rnd::rng);
            break;

        // Upon free recombination point, switch to random chromosome
        case 1u:
            hap = gethaplotype(rnd::rng);
            ++chrom;
            if (chrom < nchrom) chromend = chromends[chrom];
            assert(chrom < nchrom);
            break;

        // Upon gene, change to haplotype of the pollen
        default:

            if (hap) {
                if (pollen.getAllele(locus)) genome.set(locus);
                else genome.reset(locus);
            }

            ++locus;
            if (locus < nloci) position = locations[locus];
            break;
        }
    }

    assert(locus == nloci);
    assert(chrom == nchrom - 1u);

}

size_t Individual::getDeme() const { return deme; }
size_t Individual::getPatch() const { return patch; }
double Individual::getX() const { return x; }
bool Individual::isAlive() const { return alive; }
size_t Individual::getAllele(const size_t &l) const { return genome.test(l); }
size_t Individual::getAlleleSum() const { return genome.count(); }
