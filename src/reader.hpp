#ifndef BRACHYPODE_READER_HPP
#define BRACHYPODE_READER_HPP

// This header contains the Reader class.

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>

#include "checker.hpp"

class Reader {

public:

    // Constructor
    Reader(const std::string&);

    // Setters
    void open();
    void readline();
    void close();

    // Templated readers
    template <typename T> void readvalue(T&, const std::string& = "");
    template <typename T> void readvalues(std::vector<T>&, const size_t&, const std::string& = "", const std::string& = "");

    // Breaker
    void readerror() const;
    
    // Getters
    bool isopen() const { return file.is_open(); }
    bool iseof() { return file.peek() == std::ifstream::traits_type::eof(); }
    bool iseol() { return line.peek() != std::istringstream::traits_type::eof(); }
    bool isempty() const { return empty; }
    bool iscomment() const { return comment; }
    size_t getcount() const { return count; }
    std::string getfilename() const { return filename; }
    std::string getline() const { return line.str(); }
    std::string getname() const { return name; }

private:

    // File members
    std::string filename;
    std::ifstream file;
    
    // Line members
    bool empty;
    bool comment;
    size_t count;
    std::istringstream line;
    std::string name;

    // Private setters
    template <typename T> void read(T&, const std::string&);

    // Error messages
    std::string errorOpenFile() const;
    std::string errorEmptyFile() const;
    std::string errorReadName() const;
    std::string errorNoValue() const;
    std::string errorReadValue() const;
    std::string errorParseValue() const;
    std::string errorTooManyValues() const;
    std::string errorTooFewValues() const;
    std::string errorInvalidParameter() const;

    // Validity
    std::string errorPositive() const;
    std::string errorStrictlyPositive() const;
    std::string errorProportion() const;
    std::string errorOneToThousand() const;
    std::string errorEnoughMB() const;

    // Validity for vectors
    std::string errorStrictOrder() const;

};

// Function to read a value from the current line
template <typename T> 
void Reader::read(T &value, const std::string &valid) {

    // TODO: Remember to document arguments

    // Temporary receptacle
    std::string temp;

    // TODO: Access index?

    // Make sure the next value can be read
    if (!(line >> temp)) 
        throw std::runtime_error(errorReadValue());
        
    // Prepare to store the value
    std::istringstream stream(temp);

    // Prepare to capture leftover characters 
    char leftover;

    // Read the value and check
    if (!(stream >> value) || (stream >> leftover))
        throw std::runtime_error(errorParseValue());

    // Check validity
    if (valid == "positive" && !chk::ispositive(value)) throw std::runtime_error(errorPositive());
    else if (valid == "strictpos" && !chk::isstrictpos(value)) throw std::runtime_error(errorStrictlyPositive());
    else if (valid == "proportion" && !chk::isproportion(value)) throw std::runtime_error(errorProportion());
    else if (valid == "onetothousand" && !chk::isonetothousand(value)) throw std::runtime_error(errorOneToThousand());
    else if (valid == "isenoughmb" && !chk::isenoughmb(value)) throw std::runtime_error(errorEnoughMB());
    else return;

}

// Function to read a parameter value
template <typename T>
void Reader::readvalue(T &value, const std::string &valid) {

    // Read value in
    read(value, valid);

    // Check that we have reached the end of the line
    if (!iseol())
        throw std::runtime_error(errorTooManyValues());
    
}

// Function to read a vector of values
template <typename T>
void Reader::readvalues(std::vector<T> &values, const size_t &n, const std::string &valid, const std::string &valids) {

    // Check
    assert(n != 0);

    // Resize
    values.clear();
    values.reserve(n);

    // Counter
    size_t i = 0u;

    // While we have not reached the end of the line...
    while (!iseol()) {

        // If too many values...
        if (i == n) 
            throw std::runtime_error(errorTooManyValues());
        
        // Prepare to store the value
        T value;

        // Read the value
        read(value, valid);

        // Add to the vector
        values.push_back(value);

        // Increment value counter
        ++i;

    }

    // If too few values...
    if (i != n - 1u) 
        throw std::runtime_error(errorTooFewValues());
    
    // Check
    assert(i == n - 1u);
    assert(values.size() == n);

    // Check validity (vector level)
    if (valids == "strictorder") if (!chk::isstrictorder(values)) throw std::runtime_error(errorStrictOrder());
    else return;

}

#endif