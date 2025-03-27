#ifndef BRACHYPODE_UTILITIES_HPP
#define BRACHYPODE_UTILITIES_HPP

// This header contains the namespace Utility.

#include <stdexcept>
#include <cmath>

namespace utl {

    // Useful functions
    bool isinteger(const double&);
    double pown(double, int);
    double rootn(const double&, int, const double& = 1e-9);

}

#endif