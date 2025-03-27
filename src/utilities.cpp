// Functions of the Utility namespace.

#include "utilities.hpp"

// Function to tell if a number is an integer
bool utl::isinteger(const double &x) {

    // Is the rounded version equal to the original?
    return std::floor(x) == x;

}

// Wrapper for efficient handling of integer power
double utl::pown(double x, int n) {

    // x: value to exponentiate
    // n: exponent

    // Initialize
    int y = 1;

    // For as long as it takes...
    while (n > 0) {

        // Multiply by base if exponent is odd
        if (n % 2 == 1)  
            y *= x;

        // Square the base
        x *= x;

        // Halve the exponent
        n /= 2;

    }

    return y;

}

// Function to extract root using binary search
double utl::rootn(const double &x, int n, const double &precis) {

    // x: value to extract the root of
    // n: degree of the root

    // Degree cannot be zero
    if (n == 0) 
        std::runtime_error("Zeroeth root is undefined");

    // And only positive domain for even roots
    if (x < 0 && n % 2 == 0)
        throw std::runtime_error("Even root of negative number is not real");

    // Is the degree negative?
    const bool isneg = n < 0;

    // If so, work with positive counterpart
    if (isneg) n = std::abs(n);

    // Initialize values
    double low = 0;
    double high = x > 1.0 ? x : 1.0;

    // For as long as it takes...
    while (high - low > precis) {

        // Find the midpoint
        double mid = (low + high) / 2.0;

        // Elevate to the right power
        double midn = pown(mid, n);

        // Have we converged?
        if (std::abs(midn - x) < precis) return mid;

        // If not, narrow the search
        if (midn < x) low = mid; else high = mid;

    }

    // Final midpoint
    const double mid = (low + high) / 2.0; 

    // Reciprocal if negative degree
    if (isneg) return 1.0 / mid;

    // Otherwise return midpoint
    return mid;

}