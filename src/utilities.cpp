// Functions of the Utility namespace.

#include "utilities.hpp"

// Function to tell if an integer is even
bool utl::iseven(const int &n) {

    // Is it divisible by two?
    return n % 2 == 0;

}

// Function to tell if a number is an integer
bool utl::isinteger(const double &x) {

    // Is the rounded version equal to the original?
    return std::floor(x) == x;

}

// Function for power with integer exponent (by squaring)
double powint(double x, int n) {

    // x: value to exponentiate
    // n: exponent

    // Is the exponent negative?
    const bool isneg = n < 0;

    // Work with positive
    if (isneg) n = std::abs(n);

    // Check
    assert(n > 1);

    // Note: this function is for internal use only and
    // will not check whether the input is zero because
    // this has been done in its parent function.

    // Initialize
    double y = 1.0;

    // For as long as it takes...
    while (n > 0) {

        // Go down to even exponent
        if (!utl::iseven(n)) y *= x;

        // Square
        x *= x;

        // Decrease exponent
        n /= 2;

    }

    // Check
    assert(n == 0);

    // Reciprocal if needed
    if (isneg) return 1.0 / y;

    // Return
    return y;

}

// Wrapper around the power function for handling integral exponents
double utl::power(const double &x, const double &n) {

    // Note: the pow() function will return NaN if the power
    // is undefined.

    // Early exits to avoid loop
    if (x == 0.0) return 0.0;
    if (x == 1.0) return 1.0;
    if (n == 0.0) return 1.0;
    if (n == 1.0) return x;

    // Fast function for integer exponents
    if (isinteger(n)) return powint(x, static_cast<int>(n));

    // Otherwise use heavy duty function
    return std::pow(x, n);

}