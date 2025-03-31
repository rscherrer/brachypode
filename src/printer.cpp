// Functions pertaining to the stf namespace.

#include "printer.hpp"

// Function to convert computer memory in number of values
size_t prt::memtosize(const double &m, const double &u) {

    // m: memory use
    // u: the unit (e.g. 1 million for megabyte)

    // Check
    assert(m >= 0.0);
    assert(u >= 0.0);

    // How many values can be stored at most?
    return std::floor(m * u / sizeof(double));

}

// Constructor
Printer::Printer(const std::vector<std::string> &names, const double &mem) :
    memory(prt::memtosize(mem, 1E6)),
    outputs(names),
    valids(names),
    buffers(std::unordered_map<std::string, std::optional<Buffer> >())
{

    // names: names of the output variables
    // mem: memory use (in MB)

    // Check
    assert(memory > 0u);

}

// Search for a string in a vector of strings
bool in(const std::string &x, const std::vector<std::string> &v) {

    // x: the string to search for
    // v: the vector to search in

    // Check
    assert(!v.empty());

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

    // Check
    assert(!outputs.empty());

    // For each output...
    for (auto &name : outputs) {

        // TODO: Use find wherever possible

        // Set up a buffer
        buffers[name].emplace(Buffer(memory, name + ".dat"));

        // Open the buffer
        buffers[name]->open();

        // Check
        assert(buffers[name]->isopen());

    }

    // Check
    assert(buffers.size() == outputs.size());

}

// Function to find a buffer in the list
bool Printer::contains(const std::string &name) {

    // Early exit
    if (buffers.empty()) return false;

    // Find the right buffer
    const auto it = buffers.find(name);

    // Check if it is found
    return it != buffers.end();

}

// Function to tell if a buffer is open
bool Printer::isopen(const std::string &name) {

    // Early exit
    if (!contains(name)) return false;

    // Check if open
    return buffers[name]->isopen();

}

// Function to get the capacity of a buffer
bool Printer::isopen(const std::string &name) {

    // Early exit
    if (!contains(name)) return false;

    // Check if open
    return buffers[name]->capacity();

}

// Function to save data into a buffer
void Printer::save(const std::string &name, const double &x) {

    // name: name of the buffer in which to save
    // x: value to save

    // Early exit
    if (!contains(name)) return;

    // Check
    assert(buffers[name]->isopen());

    // Store the value if the buffer is open 
    buffers[name]->save(x);

}

// Version for an unsigned integer
void Printer::save(const std::string &name, const size_t &x) {

    // Early exit
    if (!contains(name)) return;

    // Check
    assert(buffers[name]->isopen());

    // Convert and store
    if (buffers[name]) buffers[name]->save(static_cast<double>(x));

}

// Function to close buffers
void Printer::close() {

    // Check
    assert(!outputs.empty());

    // For each buffer...
    for (auto &name : outputs) {

        // Close the buffer
        buffers[name]->close();

    }
}

// Function to check if all buffers are open
bool Printer::ison() {

    // Early exit if needed
    if (buffers.empty()) return false;

    // Check
    assert(!buffers.empty());

    // For each buffer...
    for (auto &buffer : buffers) {

        // Exit if any is found close
        if (!buffer.second->isopen()) return false;

    }

    // Otherwise say yes
    return true;

}