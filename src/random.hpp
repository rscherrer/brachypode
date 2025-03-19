#ifndef BRACHYPODE_RANDOM_HPP
#define BRACHYPODE_RANDOM_HPP

// This is the header for the namespace rnd (random). It contains aliases for
// various kinds of probability distributions as well as a random number
// generator. Note that the header rndutils.hpp (cc Hanno Hildebrandt) is 
// needed for some distributions.

// Example usage:
//
// Sample from a standard normal distribution
// double x = rnd::normal(0.0, 1.0)(rnd::rng);
//
// Prepare a distribution for later use
// auto mynormal = rnd::normal(0.0, 1.0);
//
// Sample from the distribution
// double x = mynormal(rnd::rng);

#include "rndutils.hpp"
#include <stddef.h>

namespace rnd
{

    // Probability distributions
    typedef std::discrete_distribution<size_t> discrete;
    typedef rndutils::mutable_discrete_distribution<size_t, rndutils::all_zero_policy_uni> mdiscrete; // mutable discrete
    typedef rndutils::iota_gap_sampler<size_t> iotagap; // cumulative geometric
    typedef rndutils::shuffle_sampler<size_t> samplenr; // sampling without replacement
    typedef std::uniform_int_distribution<size_t> random;
    typedef std::exponential_distribution<double> exponential;
    typedef std::binomial_distribution<size_t> binomial;
    typedef std::poisson_distribution<size_t> poisson;
    typedef std::geometric_distribution<size_t> geometric;
    typedef std::uniform_real_distribution<double> uniform;
    typedef std::normal_distribution<double> normal;
    typedef std::gamma_distribution<double> gamma;
    typedef std::bernoulli_distribution bernoulli;

    // Random number generator
    extern std::mt19937_64 rng;

}

#endif
