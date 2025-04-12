// Source code of the rdr namespace.

#include "reader.hpp"

// Constructor
Reader::Reader(const std::string &filename) : 
    filename(filename),
    file(std::ifstream()),
    empty(false),
    comment(false),
    count(0u),
    line(std::istringstream()),
    name("")
{

}

// Error messages
std::string Reader::errorOpenFile() const { return "Unable to open file " + filename; }
std::string Reader::errorEmptyFile() const { return "File " + filename + " is empty"; }
std::string Reader::errorReadName() const { return "Could not read parameter name in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorNoValue() const { return "No value for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorReadValue() const { return "Could not read value for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorParseValue() const { return "Invalid value type for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorTooManyValues() const { return "Too many values for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorTooFewValues() const { return "Too few values for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorInvalidParameter() const { return "Invalid parameter: " + name + " in line " + std::to_string(count) + " of file " + filename; }

// Error messages about validity
std::string Reader::errorPositive() const { return "Parameter " + name + " must be positive in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorStrictlyPositive() const { return "Parameter " + name + " must be strictly positive in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorProportion() const { return "Parameter " + name + " must be between 0 and 1 in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorOneToThousand() const { return "Parameter " + name + " must be between 1 and 1000 in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorEnoughMB() const { return "Parameter " + name + " must be enough MB to store a double in line " + std::to_string(count) + " of file " + filename; }

// Error message for vector validity
std::string Reader::errorStrictOrder() const { return "Parameter " + name + " must be entered in strictly increasing order in line " + std::to_string(count) + " of file " + filename; }

// Function to open the file
void Reader::open() {

    // Open the file
    file.open(filename.c_str());

    // Check if the file is open
    if (!isopen())
        throw std::runtime_error(errorOpenFile());

    // Check if the file is empty
    if (iseof())
        throw std::runtime_error(errorEmptyFile());

    // Check
    assert(isopen());
    assert(!iseof());
    assert(count == 0u);

}

// Function to read a line from the file
void Reader::readline() {

    // Check
    assert(isopen());
    assert(!iseof());

    // Temporary container
    std::string temp;

    // Read the line
    std::getline(file, temp);

    // Check if the line is empty
    empty = temp.empty();;

    // Check if the line is a comment
    comment = temp[0] == '#';

    // Clear if needed
    if (comment) temp.clear();

    // Convert the line into a stream
    line.clear();
    line.str(temp);
    line.seekg(0, std::ios::beg);

    // Increment line count
    ++count;

    // If needed...
    if (empty || comment) {
        
        // Early exit
        name.clear();
        return;

    }

    // Read parameter name
    if (!(line >> name))
        throw std::runtime_error(errorReadName());

    // Check that we are not at the end of the line
    if (iseol())
        throw std::runtime_error(errorNoValue());

}

// Function to error on invalid parameter
void Reader::readerror() const {

    // Throw error
    throw std::runtime_error(errorInvalidParameter());

}

// Function to close the input file
void Reader::close() {

    // Close
    file.close();

}