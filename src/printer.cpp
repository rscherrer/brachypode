// Functions pertaining to the stf namespace.

#include "printer.hpp"
#include <cassert>

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
void stf::read(
    std::vector<std::string> &outputs,
    const std::string &filename,
    const std::vector<std::string> &valid
) {

    // outputs: the list of outputs to update
    // filename: the name of the file to read
    // valid: the list of valid outputs

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
        if (!in(input, valid))
            throw std::runtime_error("Invalid output requested in " + filename + ": " + input);

        // Save the requested output
        outputs.push_back(input); 

    }

    // Close the file
    file.close();

}

// Function to open buffers
void stf::open(
    std::unordered_map<std::string, std::optional<Buffer> > &buffers,
    const std::vector<std::string> &names
) {

    // buffers: the buffers to open
    // names: the names of the buffers to open

    // For each output...
    for (auto &name : names) {

        // Set up a buffer
        buffers[name] = Buffer(1000u, name + ".dat");

    }

    // TODO: Make buffer size user defined
    // TODO: Say we have opened the outputs files succesfully?

}

// Function to close buffers
void stf::close(std::unordered_map<std::string, std::optional<Buffer> > &buffers) {

    // buffers: the buffers to close

    // For each buffer...
    for (auto &buffer : buffers) {

        // Close the buffer
        buffer.second->close();

    }
}