#ifndef BRACHYPODE_TESTS_TESTUTILS_HPP
#define BRACHYPODE_TESTS_TESTUTILS_HPP

// This header is for the tst (test) namespace, which contains functions used in unit tests.

#include <vector>
#include <string>
#include <functional>

namespace tst
{

    // Functions used in unit tests
    std::vector<double> read(const std::string&);
    std::string readtext(const std::string&);
    void write(const std::string&, const std::string&);
    void checkError(const std::function<void()>&, const std::string&);

}

#endif