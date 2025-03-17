// This script contains member functions for the Buffer class.

#include "buffer.hpp"
#include <cassert>

// Constructor
Buffer::Buffer(const size_t &n) : 
    first(std::vector<double>()),
    second(std::vector<double>()),
    head(&first),
    tail(&second)
{

    // n: size of the buffer

    // Reserve space
    first.reserve(n);
    second.reserve(n);

    // Check that the containers are empty
    assert(first.size() == 0u);
    assert(second.size() == 0u);

    // Check that the containers have reserved space
    assert(first.capacity() == n);
    assert(second.capacity() == n);

    // Check that the references are well established
    assert(head->capacity() == n);
    assert(tail->capacity() == n);

}

// Function to return the last value stored
double Buffer::last() const {

    // Get last value
    return head->back();

}

// Function to return the number of values stored
size_t Buffer::nstored() const {

    // Get size
    return head->size();

}

// Function to store a new value
void Buffer::store(const double &x) {

    // x: the value to store

    // Add the value to the active container
    head->push_back(x);

}

