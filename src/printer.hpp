#ifndef BRACHYPODE_PRINTER_HPP
#define BRACHYPODE_PRINTER_HPP

// This header is for the Printer class, which saves data to output files
// by means of buffers.

#include "buffer.hpp"
#include <unordered_map>
#include <optional>

class Printer {

public:

    // Constructor
    Printer(const std::vector<std::string>&);

    // Setters
    void read(const std::string&);
    void open();
    void save(const std::string&, const double&);
    void save(const std::string&, const size_t&);
    void save(const std::string&, const int&);
    void close();

    // Getters
    bool ison();
    bool check(const std::string&);

private:

    // Output variable names
    std::vector<std::string> outputs;

    // Valid names
    std::vector<std::string> valids;

    // Data buffers
    std::unordered_map<std::string, std::optional<Buffer> > buffers;

};

#endif

