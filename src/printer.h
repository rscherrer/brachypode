#ifndef BRACHYPODE_PRINTER_H
#define BRACHYPODE_PRINTER_H

//

#include <fstream>
#include <vector>
#include <memory>

namespace stf
{

    void open(std::vector<std::shared_ptr<std::ofstream> >&, const std::vector<std::string>&);
    void close(std::vector<std::shared_ptr<std::ofstream> >&);

}

#endif

