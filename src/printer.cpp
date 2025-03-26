// Functions pertaining to the stf namespace.

#include "printer.hpp"
#include <cassert>

// Constructor
Printer::Printer(const std::vector<std::string> &names) :
    outputs(names),
    valids(names),
    buffers()
{

    // names: names of the output variables

}

// Search for a string in a vector of strings
bool in(const std::string &x, const std::vector<std::string> &v) {

    // x: the string to search for
    // v: the vector to search in

    // For each element in the vector...
    for (const std::string &s : v) {

        // If the element is found, return true
        if (x == s) return true;

    }

    // If the element is not found, return false
    return false;

}

// Function to read a list of user defined outputs
void Printer::read(const std::string &filename) {

    // filename: the name of the file to read

    // Clear the list of outputs
    outputs.clear();

    // Open the file
    std::ifstream file(filename.c_str());

    // Check that the file is open
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);

    // Prepare to read in requested outputs
    std::string input;

    // For each entry...
    while (file >> input) {

        // Check that the requested output is valid
        if (!in(input, valids))
            throw std::runtime_error("Invalid output requested in " + filename + ": " + input);

        // Save the requested output
        outputs.push_back(input); 

    }

    // Close the file
    file.close();

}

// Function to open buffers
void Printer::open() {

    // For each output...
    for (auto &name : outputs) {

        // Set up a buffer
        buffers[name] = Buffer(1000u, name + ".dat");

    }

    // Check
    assert(buffers.size() == outputs.size());

    // TODO: Make buffer size user defined
    // TODO: Say we have opened the outputs files succesfully?

}

// Function to save data into a buffer
void Printer::save(const std::string &name, const double &x) {

    // name: name of the buffer in which to save
    // x: value to save

    // Store the value if the buffer is open 
    if (buffers[name]) buffers[name]->save(x);

}

// Version for an integer
void Printer::save(const std::string &name, const int &x) {

    // Convert and store
    if (buffers[name]) buffers[name]->save(static_cast<double>(x));

}

// Version for an unsigned integer
void Printer::save(const std::string &name, const size_t &x) {

    // Convert and store
    if (buffers[name]) buffers[name]->save(static_cast<double>(x));

}

// Function to close buffers
void Printer::close() {

    // For each buffer...
    for (auto &buffer : buffers) {

        // Close the buffer
        buffer.second->close();

    }
}

// Function to check if a particular buffer is open
bool Printer::check(const std::string &name) { return buffers[name]->isopen(); }

// Function to check if all buffers are open
bool Printer::ison() {

    // For each buffer...
    for (auto &buffer : buffers) {

        // Exit if any is found close
        if (!buffer.second->isopen()) return false;

    }

    // Otherwise say yes
    return true;

}