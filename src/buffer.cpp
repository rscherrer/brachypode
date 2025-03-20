// This script contains member functions for the Buffer class.

#include "buffer.hpp"
#include <cassert>
#include <fstream>
#include <algorithm>

// Constructor
Buffer::Buffer(const size_t &n, const std::string &filename) :
    maxsize(n),
    head(std::make_unique<std::vector<double> >()),
    tail(std::make_unique<std::vector<double> >()),
    file(std::ofstream(filename.c_str(), std::ios::binary))
{

    // n: size of the buffer
    // filename: name of the ouput file 

    // Reserve space in the containers
    reserve(maxsize);

    // Check that the containers are empty
    assert(head->size() == 0u);
    assert(tail->size() == 0u);

    // Check that the file is open
    if (!file.is_open()) 
        throw std::runtime_error("Unable to open file " + filename);

}

// Function to return the last value stored
double Buffer::last() const {

    // Get last value
    return head->back();

}

// Function to return the number of values stored
size_t Buffer::size() const {

    // Get size
    return head->size();

}

// Function to check if the file is open
bool Buffer::isopen() const {

    // Check if the file is open
    return file.is_open();

}

// Function to reserve space in the buffer
void Buffer::reserve(const size_t &n) {

    // n: size of the buffer

    // Reserve space
    head->reserve(n);
    tail->reserve(n);

    // Check that the containers have reserved space
    assert(head->capacity() == n);
    assert(tail->capacity() == n);

}

// Function to store a new value
void Buffer::store(const double &x) {

    // x: the value to store

    // Check that we are below storage capacity
    assert(head->size() < maxsize);

    // Add the value to the active container
    head->push_back(x);

    // If the maximum allowed capacity is reached...
    if (head->size() == maxsize) flush();

}

// Function to write all the content of the buffer to file
void Buffer::flush() {

    // Make sure the file is open
    assert(file.is_open());

    // Swap the head and tail of the buffer
    std::swap(head, tail);

    // Write every stored value to file
    file.write((char *) tail->data(), tail->size() * sizeof(double));

    // Empty the tail
    tail->clear();

    // Make sure the buffer is empty
    assert(head->size() == 0u);
    assert(tail->size() == 0u);

}

// Function to close the output data file
void Buffer::close() {

    // Check that the file is open
    assert(file.is_open());

    // Close the file
    file.close();

    // Check that the file is closed
    assert(!file.is_open());

}