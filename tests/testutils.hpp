#ifndef BRACHYPODE_TESTS_TESTUTILS_HPP
#define BRACHYPODE_TESTS_TESTUTILS_HPP

// This header is for the tst (test) namespace, which contains functions used in unit tests.

#include <vector>
#include <string>

namespace tst
{

    // Functions used in unit tests
    std::vector<double> readBinary(const std::string&);

}

#endif