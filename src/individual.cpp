#include "individual.h"

#include <cmath>

Individual::Individual() :
    genome(std::bitset<1000>()),
    patch(0u),
    tolerance(6.0),
    competitiveness(1.0),
    neutral(0.0),
    fitness(1.0),
    alive(true)
{

    assert(genome.count() == 0u);

}

// Function to throw mutations across the genome
void Individual::mutateGenome(const double &mu, const size_t &n) {

    // mu = mutation rate
    // n = number of loci

    if (mu == 0.0) {

        return;

    } else if (mu == 1.0) {

        for (size_t i = 0u; i < 2u * n; ++i)
            genome.set(i);

    } else if (mu > 0.1) {

        // Use a bernoulli if common
        auto isMutation = rnd::bernoulli(mu);
        for (size_t i = 0u; i < 2u * n; ++i)
            if (isMutation(rnd::rng)) genome.set(i);

    } else {

        // Otherwise sample mutations from a geometric distribution
        auto getNextMutant = rnd::iotagap(mu);
        getNextMutant.reset(0u);

        for (;;) {

            const size_t mut = getNextMutant(rnd::rng);
            if (mut >= 2.0 * n) break;
            genome.flip(mut);

        }
    }
}

void Individual::inherit(const Individual &gamete, const Parameters &p, const Architecture &a, const bool &sex) {

    size_t l = 0u;
    size_t k = 0u;

    // Haplotypes have equal chances to be transmitted
    auto getHaplotype = rnd::bernoulli(0.5);

    // Crossovers are sampled from an exponential distribution
    const double recombrate = p.recombination > 0.0 ? p.recombination : 100.0;
    auto nextCrossover = rnd::exponential(recombrate);

    double crossover = 1.1; // beyond the end of the genome
    if (p.recombination > 0.0) crossover = nextCrossover(rnd::rng);

    double position = a.getLocation(0u);
    double chromend = a.getChromEnd(0u);

    size_t hap = getHaplotype(rnd::rng);

    while (l < a.getNLoci()) {

        // What is the thing coming up next?
        size_t next = static_cast<size_t>(crossover);
        if (crossover < chromend && crossover < position)
            next = 0u;
        else if (chromend < crossover && chromend < position)
            next = 1u;
        else
            next = 2u;

        switch (next) {

        // Upon crossover point, switch haplotype
        case 0u:
            hap = hap ? 0u : 1u;
            crossover += nextCrossover(rnd::rng);
            break;

        // Upon free recombination point, switch to random chromosome
        case 1u:
            hap = getHaplotype(rnd::rng);
            ++k;
            if (k < p.nchrom) chromend = a.getChromEnd(k);
            assert(k < p.nchrom);
            break;

        // Upon gene, transmit haplotype to the zygote
        default:
            assert(l + hap * a.getNLoci() < 2u * a.getNLoci());
            assert(l + sex * a.getNLoci() < 2u * a.getNLoci());
            if (gamete.readAllele(l + hap * a.getNLoci()))
                genome.set(l + sex * a.getNLoci());
            ++l;
            if (l < a.getNLoci()) position = a.getLocation(l);
            break;
        }
    }

    assert(l == a.getNLoci());
    assert(k == p.nchrom - 1u);

}

// Function to set trait values from the genome
void Individual::develop(const Architecture &a) {

    tolerance = 0.0;
    competitiveness = 1.0;
    neutral = 0.0;

    for (size_t l = 0u; l < a.getNLoci(); ++l) {

        const size_t trait = a.getTrait(l);
        const double effect = a.getEffect(l);

        const size_t genotype = genome.test(l) + genome.test(l + 3u);
        const double expression = genotype * 0.5;
        const double contribution = expression * effect;

        switch (trait) {

        case 0u: tolerance += contribution; break;
        case 1u: competitiveness += contribution; break;
        case 2u: neutral += contribution; break;
        default: throw std::runtime_error("Unknown trait encoded by one locus"); break;

        }
    }

    assert(tolerance >= 0.0);
    assert(competitiveness >= 0.0);
    assert(neutral >= 0.0);

}

void Individual::setFitness(const Parameters &p, const Landscape &l, const double &density) {

    // Local environmental conditions
    const double stress = l.getStress(patch);
    const double competition = l.getCompetition(patch);
    const size_t habitat = l.getHabitat(patch);

    // Compute stress-dependent growth
    double F = 1.0 - 1.0 / (1.0 + exp(-p.steepness * (stress - tolerance)));
    //F = 1.0;

    // Compute competition-dependent growth (no competition in the matrix)
    double G = habitat ? exp(-competition * (density / competitiveness - 1.0)) : 1.0;
    //G = 1.0;

    // Compute cost-depdendent growth
    double C = exp(-(p.costcomp * competitiveness * competitiveness + p.tradeoff * competitiveness * tolerance + p.costtol * tolerance * tolerance));
    //C = 1.0;

    fitness = p.maxgrowth * F * G * C;

}

void Individual::setPatch(const size_t &p) { patch = p; }
void Individual::setTolerance(const double &x) { tolerance = x; }
void Individual::setCompetitiveness(const double &x) { competitiveness = x; }
void Individual::kill() { alive = false; }

size_t Individual::getPatch() const { return patch; }
double Individual::getTolerance() const { return tolerance; }
double Individual::getCompetitiveness() const { return competitiveness; }
double Individual::getNeutral() const { return neutral; }
double Individual::getFitness() const { return fitness; }
bool Individual::isAlive() const { return alive; }
size_t Individual::getNOneAlleles() const { return genome.count(); }
bool Individual::readAllele(const size_t &i) const { return genome.test(i); }
