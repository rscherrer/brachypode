#ifndef BRACHYPODE_BUFFER_HPP
#define BRACHYPODE_BUFFER_HPP

// This is the header for the data buffer class, which stores data in temporary containers
// before flusing it into some output file.

#include <vector>

class Buffer {

public:

    // Constructor
    Buffer(const size_t&);

    // Getters
    double last() const;
    size_t nstored() const;

    // Setters
    void store(const double&);

private:

    // Data containers
    std::vector<double> first;
    std::vector<double> second;

    // Pointers to be swapped regularly
    std::vector<double>* head;
    std::vector<double>* tail;

};

#endif