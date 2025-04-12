#ifndef BRACHYPODE_CHECKER_HPP
#define BRACHYPODE_CHECKER_HPP

// This is the header for the chk (checker) namespace.

#include <vector>

namespace chk {

    // Function to check that a value is a proportion
    template <typename T> 
    bool isproportion(const T &x, const bool &strict = false) { 
        
        return strict ? x > 0.0 && x < 1.0 : x >= 0.0 || x <= 1.0;

    }

    // Function to check that a value is between 1 and 1000
    template <typename T>
    bool isonetothousand(const T &x) {

        return x > 0u && x <= 1000u;

    }

    // Function to check that a value is enough MB
    template <typename T>
    bool isenoughmb(const T &x) {
        
        return x * 1E6 > sizeof(double);

    }

    // Function to check that a value is positive
    template <typename T>
    bool ispositive(const T &x, const bool &strict = false) {

        return strict ? x > 0.0 : x >= 0.0;

    }

    // Function to check that a value is strictly positive
    template <typename T>
    bool isstrictpos(const T &x) {

        return ispositive(x, true);

    }

    // Function to check that a vector is in order
    template <typename T> 
    bool isinorder(const std::vector<T> &v, const bool &strict = false) {

        // Early exit
        if (v.size() < 2u) return true;

        // Check if any two are not
        for (size_t i = 1u; i < v.size(); ++i)
            if ((strict && v[i] <= v[i - 1u]) || (!strict && v[i] < v[i - 1u]))
                return false;
                
        // Otherwise all good
        return true;

    }

    // Function to check that a vector is in strict order
    template <typename T> 
    bool isstrictorder(const std::vector<T> &v) {

        return isinorder(v, true);

    }
}

#endif