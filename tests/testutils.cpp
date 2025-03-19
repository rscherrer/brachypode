// Functions pertaining to the tst namespace.

#include "testutils.hpp"
#include <fstream>

// Function to read a binary data file
std::vector<double> tst::readBinary(const std::string &filename)
{
    // Open the input file
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

    // Prepare storage for values
    double x;
    std::vector<double> v;

    // If the file is open
    if (file.is_open()) {

        // Loop through the file until we reach the end of the file
        while(file) {

            // Read elements
            file.read((char *) &x, sizeof(size_t));

            // Exit if reaching the end of the file
            if (!file.gcount()) break;

            // Store elements
            v.push_back(x);

        }
    }

    // Close the file
    file.close();

    return v;

}