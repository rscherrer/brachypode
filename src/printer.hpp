#ifndef BRACHYPODE_PRINTER_HPP
#define BRACHYPODE_PRINTER_HPP

// This header is for the stf (save to file) namespace, which contains functions to save data to file.

#include "buffer.hpp"
#include <fstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>

namespace stf {

    // Functions to help save to file
    void read(std::vector<std::string>&, const std::string&, const std::vector<std::string>&);
    void open(std::unordered_map<std::string, std::optional<Buffer> >&, const std::vector<std::string>&);
    void close(std::unordered_map<std::string, std::optional<Buffer> >&);

}

#endif

