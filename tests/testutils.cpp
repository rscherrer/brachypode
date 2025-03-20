// Functions pertaining to the tst namespace.

#include "testutils.hpp"
#include <fstream>
#include <functional>
#include <boost/test/unit_test.hpp>

// Function to read a binary data file
std::vector<double> tst::read(const std::string &filename)
{

    // filename: the name of the file to read

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

// Function to write a text file
void tst::write(const std::string &filename, const std::string &content)
{

    // filename: the name of the file to write
    // content: the content to write to the file

    // Open the output file
    std::ofstream file(filename);

    // Check if the file is open
    if (!file.is_open()) 
        throw std::runtime_error("Unable to open file " + filename);

    // Write the content to the file
    file << content;

    // Close the file
    file.close();

}

// Helper function to check for errors and error messages
void tst::checkError(const std::function<void()>& func, const std::string& expected) {

    // func: function to run
    // expected: expected error message

    // Prepare to catch an error
    bool error = false;
    std::string message = "";

    // Try to...
    try {

        // Run the function
        func();

    }
    catch (const std::runtime_error& err) {

        // Catch the error
        error = true;
        message = err.what();

    }
    
    // Check that an error occurred
    BOOST_CHECK(error);

    // Check that the error message is the expected one
    BOOST_CHECK_EQUAL(message, expected);

}