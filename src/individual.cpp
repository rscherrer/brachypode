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

void Individual::setFitness(const Param &p, const Landscape &l, const double &density) {

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
