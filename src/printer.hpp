#ifndef BRACHYPODE_PRINTER_HPP
#define BRACHYPODE_PRINTER_HPP

// This header is for the Printer class. The Printer class is used to save data
// from the simulation into output files. The class has flags to know which data
// to save, and it has methods to save the data. The data are saved in binary format
// to speed up the process.

#include <fstream>
#include <vector>
#include <memory>

class Printer {

public:

    // Constructor
    Printer(const bool&);

private:

    // Flags for the different types of data to save
    bool time;
    bool popsize;
    bool patchsizes;
    bool traitmeans;
    bool individuals;

};

#endif

