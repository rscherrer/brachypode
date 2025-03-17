#ifndef BRACHYPODE_PRINTER_HPP
#define BRACHYPODE_PRINTER_HPP

// This header contains functions for writing data to output file.

#include <fstream>
#include <vector>
#include <memory>

namespace stf
{

    // Printing functions (stf stands for "save to file")
    void open(std::vector<std::shared_ptr<std::ofstream> >&, const std::vector<std::string>&);
    void close(std::vector<std::shared_ptr<std::ofstream> >&);
    void check(const std::vector<std::string>&, const std::vector<std::string>&);

}

#endif

