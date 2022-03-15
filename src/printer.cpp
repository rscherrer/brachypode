//

#include "printer.h"

// Function to add and open output file streams to a vector of streams
void stf::open(
    std::vector<std::shared_ptr<std::ofstream> > &outfiles,
    const std::vector<std::string> &filenames
) {

    // Reserve space for enough output files
    outfiles.reserve(filenames.size());

    // For each file...
    for (size_t f = 0u; f < filenames.size(); ++f) {

        // Add extension to the file name
        const std::string filename = filenames[f] + ".dat";

        // Create a new output stream
        std::shared_ptr<std::ofstream> out(new std::ofstream);

        // Open the output stream (data are written in binary format)
        out->open(filename.c_str(), std::ios::binary);

        // Check
        if (!out->is_open()) {
            std::string msg = "Unable to open output file " + filename;
            throw std::runtime_error(msg);
        }

        // Add the stream to the vector of streams
        outfiles.push_back(out);

    }
}

// Function to close all the file streams
void stf::close(std::vector<std::shared_ptr<std::ofstream> > &outfiles) {

    for (size_t f = 0u; f < outfiles.size(); ++f)
        outfiles[f]->close();

}


