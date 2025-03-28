// This file contains function definitions for class Population.

#include "population.hpp"

// Constructor
Population::Population(const Parameters &pars, const Architecture &arch) :
    individuals(std::make_unique<std::vector<Individual> >()),
    newborns(std::make_unique<std::vector<Individual> >()),
    ndemes(pars.pgood.size()),
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
    assert(newborns->size() == 0u);

    // Sow individuals at random if needed
    if (pars.sow) shuffle();

}

// Function to sow individuals at random across demes
void Population::shuffle() {

    // Prepare a random deme sampler
    auto pickDeme = rnd::random(0u, ndemes - 1u);

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
double lincrement(const double &x, const double &xfinal, const int &t, const int &tfinal) {

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

    // Early exit if warming has not started
    if (time <= tchange) return;

    // Time at which climate change ends
    const size_t tfinal = tchange + twarming;

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
    assert(pgood.size() != 0u);
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
    assert(tend != 0u);
    assert(tsave != 0u);
    assert(demesizes.size() == ndemes);
    assert(patchsizes.size() == 2u * ndemes);
    assert(meantol.size() == 2u * ndemes);
    assert(time >= 0);

}

// TODO: Then effect sizes MUST be srictly positive

// Function to perform one step of the life cycle
void Population::cycle(Printer &print) {

    // print: a printer

    // TODO: See if we can delegate some of that stuff to test better

    // Make sure the population is not extinct
    assert(individuals->size() > 0u);

    // Reset statistics
    reset();

    // Flag to know if it is time to save some data
    const bool tts = print.ison() && time % tsave == 0u;

    // Save time step if needed
    if (tts) print.save("time", time);

    // Prepare to record the number of new individuals
    size_t totseeds = 0u;

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

    // For each individual...
    for (Individual& ind : *individuals) {

        // Get trait values and locations
        const double tol = ind.getTolerance();
        const size_t deme = ind.getDeme();
        const size_t patch = ind.getPatch();

        // Current local population size
        const size_t n = patchsizes[2u * deme + patch];

        // Compute growth rate
        const double r = maxgrowth - tradeoff * tolmax * utl::power(tol / tolmax, nonlinear);

        // Check
        assert(!std::isnan(r));
        assert(r <= maxgrowth);
        assert(r >= maxgrowth - tradeoff * tolmax);

        // Cover of the focal patch in the deme
        const double cover = patch ? pgood[deme] : 1.0 - pgood[deme];

        // Check
        assert(cover >= 0.0);
        assert(cover <= 1.0);

        // Total carrying capacity in the deme for the focal patch
        const double Ktot = capacities[patch] * cover;

        // Check
        assert(Ktot >= 0.0);

        // Expected number of seeds
        const double enseeds = exp(r * (1.0 - n / Ktot));

        // Check
        assert(enseeds >= 0.0);

        // Realized number of seeds
        const size_t nseeds = rnd::poisson(enseeds)(rnd::rng);

        // Record the number of seeds that will be produced
        ind.setNSeeds(nseeds);

        // Cumulative sum
        totseeds += nseeds;

    }

    // Prepare space to welcome newborns
    newborns->reserve(totseeds);

    // Prepare a partner sampler for outcrossing
    auto getPollen = rnd::random(1u, popsize - 1u);

    // Prepare a deme sampler for seed dispersal
    auto getDestination = rnd::random(1u, ndemes - 1u);

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
                if (k < i) k -= 1u;

                // Recombine the genomes of the two parents
                newborns->back().recombine(recombination, (*individuals)[k]);

            }

            // If the seed disperses to another site...
            if (ndemes > 1u && rnd::bernoulli(dispersal)(rnd::rng)) {

                // Sample destination deme
                size_t newdeme = getDestination(rnd::rng);

                // Avoid the current deme
                if (newdeme < ind.getDeme()) newdeme -= 1u;

                // Send the seed there
                newborns->back().setDeme(newdeme);

            }

            // Prepare to sample landing patch based on good patch cover
            auto pickPatch = rnd::bernoulli(pgood[newborns->back().getDeme()]);

            // Set the patch of the seed
            newborns->back().setPatch(pickPatch(rnd::rng));

            // Does the seed mutate?
            newborns->back().mutate(mutation);

            // Patch where the seed has landed
            const size_t seedpatch = newborns->back().getPatch();

            // Trait value of the seedling
            const double xoff = newborns->back().getTolerance();

            // Compute the survival probability of the seedling
            const double prob = 1.0 / (1.0 + exp(steep * (stress[seedpatch] - xoff)));

            // Remove the seedling if it does not survive
            if (!rnd::bernoulli(prob)(rnd::rng)) newborns->pop_back();

        }
    }

    // Newborns become adults
    std::swap(individuals, newborns);

    // No more newborns (adults die) 
    newborns->clear();

    // Check
    assert(newborns->size() == 0u);

}

// Function to increment time
void Population::moveon() {

    // Update time
    ++time;

}

// Function to tell if the population has gone extinct
bool Population::extinct() const {

    // Is the population still there?
    if (individuals->size() == 0u) {

        // If not say it
        std::cout << "Population went extinct at t = " << time << '\n';

        // Return true
        return true;

    }

    // Otherwise return false
    return false;
}

// Function to tell whether to keep on simulating
bool Population::keepon() const { return time < tend; }

// Variable getters
size_t Population::size() const { return individuals->size(); }
size_t Population::getTime() const { return time; }

// TODO: Define simple getters in header file for efficient inlining

// Parameter getters
double Population::getPGood(const size_t &deme) const { return pgood[deme]; }
double Population::getStress(const size_t &patch) const { return stress[patch]; }
double Population::getCapacity(const size_t &patch) const { return capacities[patch]; }

// Individual getters
size_t Population::deme(const size_t &i) const { return (*individuals)[i].getDeme(); }