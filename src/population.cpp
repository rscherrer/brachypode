// This file contains function definitions for class Population.

#include "population.hpp"

// Constructor
Population::Population(const Parameters &pars, const Architecture &arch) :
    individuals(std::make_unique<std::vector<Individual> >()),
    newborns(std::make_unique<std::vector<Individual> >()),
    ndemes(pars.ndemes),
    pgood(pars.pgood),
    pgoodEnd(pars.pgoodEnd),
    stress(pars.stress),
    stressEnd(pars.stressEnd),
    capacities(pars.capacities),
    capacitiesEnd(pars.capacitiesEnd),
    maxgrowth(pars.maxgrowth),
    steep(pars.steep),
    dispersal(pars.dispersal),
    mutation(pars.mutation),
    tradeoff(pars.tradeoff),
    nonlinear(pars.nonlinear),
    selfing(pars.selfing),
    recombination(pars.recombination),
    tolmax(arch.tolmax),
    minrealk(pars.minrealk),
    tend(pars.tend),
    tsave(pars.tsave),
    tchange(pars.tchange),
    twarming(pars.twarming),
    verbose(pars.verbose),
    time(0u),
    popsize(pars.popsize),
    demesizes(std::vector<size_t>(ndemes, 0u)),
    patchsizes(std::vector<size_t>(2u * ndemes, 0u)),
    meantol(std::vector<double>(2u * ndemes, 0.0))
{

    // pars: parameter set
    // arch: genetic architecture

    // Check
    check();

    // Reserve space for the population
    individuals->reserve(popsize);
    newborns->reserve(0u);

    // Check
    assert(individuals->capacity() == popsize);
    assert(newborns->capacity() == 0u);

    // Create a shared pointer the genetic architecture
    auto pointarch = std::make_shared<Architecture>(arch);

    // Fill the population with individuals
    for (size_t i = 0u; i < popsize; ++i) 
        individuals->emplace_back(Individual(pars.allfreq, pointarch));

    // Check
    assert(individuals->size() == popsize);
    assert(newborns->empty());

    // Sow individuals at random if needed
    if (pars.sow) shuffle();

}

// Function to sow individuals at random across demes
void Population::shuffle() {

    // Prepare a random deme sampler
    auto pickDeme = rnd::random(0u, ndemes - 1u);

    // Check
    assert(!individuals->empty());

    // For each individual...
    for (Individual& ind : *individuals) {

        // Sample a deme 
        const size_t newdeme = pickDeme(rnd::rng);

        // Prepare a patch sampler based on good patch cover
        auto pickPatch = rnd::bernoulli(pgood[newdeme]);

        // Sample a patch
        const size_t newpatch = pickPatch(rnd::rng);

        // Update the individual
        ind.setDeme(newdeme);
        ind.setPatch(newpatch);

    }
}

// Function to linearly update a parameter value upon time step increment
double lincrement(const double &x, const double &xfinal, const size_t &t, const size_t &tfinal) {

    // x: current value of the parameter
    // xfinal: final value the parameter is supposed to reach
    // t: current time step
    // tfinal: time step when the parameter should reach its final value

    // Compute new value
    return x + (xfinal - x) / (tfinal - t + 1.0);

    // Note: this follows from the equation of a line.

}

// Update climate-related parameters
void Population::update() {

    // TODO: Explain that at tchange the climate has not changed yet

    // Early exit if warming has not started
    if (time <= tchange) return;

    // Check
    assert(twarming > 0u);

    // Time at which climate change ends
    const size_t tfinal = tchange + twarming;

    // Check
    assert(tfinal > tchange);

    // Early exit if warming is over
    if (time > tfinal) return;

    // For each patch...
    for (size_t i = 0u; i < 2u; ++i) {

        // Update the stress level and carrying capacity
        stress[i] = lincrement(stress[i], stressEnd[i], time, tfinal);
        capacities[i] = lincrement(capacities[i], capacitiesEnd[i], time, tfinal);

        // Check that they are still positive
        assert(stress[i] >= 0.0);
        assert(capacities[i] > 0.0);

    }

    // For each deme...
    for (size_t i = 0u; i < pgood.size(); ++i) {

        // Update the cover of good patches
        pgood[i] = lincrement(pgood[i], pgoodEnd[i], time, tfinal);

        // Make sure it is still between zero and one
        assert(pgood[i] >= 0.0);
        assert(pgood[i] <= 1.0);

    }
}

// Function to reset population statistics
void Population::reset() {

    // Reset
    popsize = 0u;
    demesizes = std::vector<size_t>(ndemes, 0u);
    patchsizes = std::vector<size_t>(2u * ndemes, 0u);
    meantol = std::vector<double>(2u * ndemes, 0.0);
    
}

// Function to gather individual statistics
void Population::gather(const size_t &deme, const size_t &patch, const double &tol) {

    // deme: the deme of the individual
    // patch: its patch
    // tol: its tolerance value

    // Patch identifier
    const size_t j = 2u * deme + patch;

    // Update population size
    ++popsize;
    
    // Update local density in the deme
    ++demesizes[deme];

    // Update local density in the patch in the deme
    ++patchsizes[j];

    // Update mean trait in the patch in the deme
    meantol[j] += tol;

}

// Function to display progress
void Population::show() const {

    // Display
    std::cout << "n = { ";

    // Each deme separately
    for (size_t j = 0u; j < demesizes.size(); ++j) std::cout << demesizes[j] << ' ';

    // Finalize
    std::cout << "} at t = " << time << '\n';

}

// Function to check parameter values
void Population::check() const {

    // Check that the parameter values are valid
    assert(popsize != 0u);
    assert(ndemes != 0u);
    assert(pgood.size() == pgoodEnd.size());
    assert(pgood.size() == ndemes);
    for (size_t i = 0u; i < pgood.size(); ++i) {
        assert(pgood[i] >= 0.0);
        assert(pgood[i] <= 1.0);
        assert(pgoodEnd[i] >= 0.0);
        assert(pgoodEnd[i] <= 1.0);
    }
    for (size_t i = 0u; i < 2u; ++i) {
        assert(capacities[i] > 0.0);
        assert(capacitiesEnd[i] > 0.0);
        assert(stress[i] >= 0.0);
        assert(stressEnd[i] >= 0.0);
    }
    assert(maxgrowth >= 0.0);
    assert(steep >= 0.0);
    assert(dispersal >= 0.0);
    assert(dispersal <= 1.0);
    assert(mutation >= 0.0);
    assert(mutation <= 1.0);
    assert(tradeoff >= 0.0);
    assert(nonlinear > 0.0);
    assert(selfing >= 0.0);
    assert(selfing <= 1.0);
    assert(recombination >= 0.0);
    assert(tolmax > 0.0);
    assert(minrealk > 0.0);
    assert(tend != 0u);
    assert(tsave != 0u);
    assert(demesizes.size() == ndemes);
    assert(patchsizes.size() == 2u * ndemes);
    assert(meantol.size() == 2u * ndemes);
    assert(time >= 0);

}

// Accessory function to compute the intrinsic population growth rate
double pop::growth(
    const double &x, const double &xmax, const double &rmax, 
    const double &epsilon, const double &nu
) {

    // x: trait value
    // xmax: maximum trait value
    // rmax: maximum growth rate
    // epsilon: trade-off strength
    // nu: trade-off non-linearity

    // Check
    assert(xmax != 0.0);

    // Compute growth rate
    double r = rmax - epsilon * xmax * utl::power(x / xmax, nu);

    // Clamp it to zero
    r = r < 0.0 ? 0.0 : r;

    // Check
    assert(r >= 0.0);
    assert(r <= rmax);
    assert(r >= rmax - epsilon * xmax);

    // Exit
    return r;

}

// Accessory function to compute fitness according to Ricker
double pop::ricker(const size_t &n, const double &r, const double &K) {

    // n: population density
    // r: per capita growth rate
    // K: carrying capacity
    
    // Check
    assert(K > 0.0);
    assert(r >= 0.0);

    // Compute fitness
    const double fitness = std::exp(r * (1.0 - n / K));

    // Check
    assert(fitness >= 0.0);

    // Exit
    return fitness;

}

// Accessory function for computing survival probability
double pop::survival(const double &x, const double &theta, const double &a) {

    // x: trait value
    // theta: stress level
    // a: steepness of the survival function

    // Check
    assert(x >= 0.0);
    assert(theta >= 0.0);
    assert(a >= 0.0);    

    // Compute probability
    const double prob = 1.0 / (1.0 + std::exp(a * (theta - x)));

    // Check
    assert(prob >= 0.0);
    assert(prob <= 1.0);

    // Exit
    return prob;

}

// Function to perform one step of the life cycle
void Population::cycle(Printer &print) {

    // print: a printer

    // Make sure the population is not extinct
    assert(!individuals->empty());

    // Reset statistics
    reset();

    // Flag to know if it is time to save some data
    const bool tts = print.ison() && time % tsave == 0u;

    // Save time step if needed
    if (tts) print.save("time", time);

    // Check
    assert(!individuals->empty());

    // For each individual...
    for (Individual& ind : *individuals) {

        // Get trait values and locations
        const double tol = ind.getTolerance();
        const size_t deme = ind.getDeme();
        const size_t patch = ind.getPatch();

        // If needed...
        if (tts) {

            // Save values
            print.save("individuals", deme);
            print.save("individuals", patch);
            print.save("individuals", tol);

        }

        // Gather individual statistics
        gather(deme, patch, tol);

    }

    // Verbose if needed
    if (verbose) show();

    // Save population size if needed
    if (tts) print.save("popsize", popsize);

    // For each patch in each deme...
    for (size_t j = 0u; j < patchsizes.size(); ++j) {

        // Finalize the computation of the mean
        if (patchsizes[j]) meantol[j] /= patchsizes[j];

        // If needed...
        if (tts) {

            // Save patch size and mean trait if needed
            print.save("patchsizes", patchsizes[j]);
            print.save("traitmeans", meantol[j]);

        }
    }

    // Prepare to record the number of new individuals
    size_t totseeds = 0u;

    // Check
    assert(!individuals->empty());

    // For each individual...
    for (Individual& ind : *individuals) {

        // Get trait values and locations
        const double tol = ind.getTolerance();
        const size_t deme = ind.getDeme();
        const size_t patch = ind.getPatch();

        // Compute growth rate
        const double r = pop::growth(tol, tolmax, maxgrowth, tradeoff, nonlinear);

        // Check
        assert(!std::isnan(r));
        assert(r <= maxgrowth);
        assert(r >= maxgrowth - tradeoff * tolmax);
        assert(r >= 0.0);

        // Cover of the focal patch in the deme
        const double cover = patch ? pgood[deme] : 1.0 - pgood[deme];

        // Check
        assert(cover >= 0.0);
        assert(cover <= 1.0);

        // Total carrying capacity in the deme for the focal patch
        double Ktot = capacities[patch] * cover;

        // Clamp
        Ktot = Ktot < minrealk ? minrealk : Ktot;

        // Check
        assert(Ktot > 0.0);

        // Current local population size
        const size_t n = patchsizes[2u * deme + patch];

        // Expected number of seeds
        const double fitness = pop::ricker(n, r, Ktot);

        // Check
        assert(fitness >= 0.0);
        assert(fitness <= std::exp(r));

        // Realized number of seeds
        const size_t nseeds = rnd::poisson(fitness)(rnd::rng);

        // Record the number of seeds that will be produced
        ind.setNSeeds(nseeds);

        // Cumulative sum
        totseeds += nseeds;

    }

    // Prepare space to welcome newborns
    newborns->reserve(totseeds);

    // Check
    assert(popsize == individuals->size());
    assert(ndemes == pgood.size());

    // Prepare a partner sampler for outcrossing
    auto getPollen = rnd::random(0u, popsize - 1u - (popsize > 1u));

    // Prepare a deme sampler for seed dispersal
    auto getDestination = rnd::random(0u, ndemes - 1u - (ndemes > 1u));

    // Note: we set those distributions this way to avoid self.

    // For each adult plant...
    for (size_t i = 0u; i < individuals->size(); ++i) {

        // Isolate focal individual
        Individual& ind = (*individuals)[i];

        // For each seed produced...
        for (size_t j = 0u; j < ind.getNSeeds(); ++j) {

            // Add a clone of the parent to the population
            newborns->push_back(ind);

            // If the seed is the product of outcrossing...
            if (rnd::bernoulli(1.0 - selfing)(rnd::rng)) {

                // Select another adult at random to provide pollen
                size_t k = getPollen(rnd::rng);

                // Avoid selfing
                k = k + (k >= i);

                // Check
                assert(k != i);

                // Recombine the genomes of the two parents
                newborns->back().recombine(recombination, (*individuals)[k]);

            }

            // Disperse the seed to another site if needed...
            if (ndemes > 1u && rnd::bernoulli(dispersal)(rnd::rng)) {

                // Sample destination deme
                size_t newdeme = getDestination(rnd::rng);

                // Avoid the current deme
                newdeme = newdeme + (newdeme >= newborns->back().getDeme());
                
                // Check
                assert(newdeme != newborns->back().getDeme());

                // Send the seed there
                newborns->back().setDeme(newdeme);

            }

            // Prepare to sample landing patch based on good patch cover
            auto pickPatch = rnd::bernoulli(pgood[newborns->back().getDeme()]);

            // Set the patch of the seed
            newborns->back().setPatch(pickPatch(rnd::rng));

            // Does the seed mutate?
            newborns->back().mutate(mutation);

            // Check
            newborns->back().check(ndemes);

            // Patch where the seed has landed
            const size_t seedpatch = newborns->back().getPatch();

            // Trait value of the seedling
            const double xseed = newborns->back().getTolerance();

            // Compute the survival probability of the seedling
            const double prob = pop::survival(xseed, stress[seedpatch], steep);

            // Check
            assert(prob >= 0.0);
            assert(prob <= 1.0);

            // Remove the seedling if it does not survive
            if (!rnd::bernoulli(prob)(rnd::rng)) newborns->pop_back();

        }
    }

    // Newborns become adults
    std::swap(individuals, newborns);

    // No more newborns (adults die) 
    newborns->clear();

    // Check
    assert(newborns->empty());

}

// Function to increment time
void Population::moveon() {

    // Update time
    ++time;

}