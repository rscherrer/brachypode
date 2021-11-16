#include "tests/test_utilities.h"

std::vector<double> tst::readfile(const std::string &filename)
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
      file.read((char *) &x, sizeof(double));

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

std::vector<size_t> tst::readfile2(const std::string &filename)
{
    // Open the input file
  std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

    // Prepare storage for values
  size_t x;
  std::vector<size_t> v;

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

void tst::makeValidLandscapeFile()
{
  std::ofstream file;
  file.open("landscape.txt");
  if (!file.is_open())
    std::cout << "Unable to open valid parameter test file.\n";

  file << 3u << '\n'
  << 10.3 << '\t' << 2.0 << '\t' << 1.0 << '\n'
  << 50u << '\t' << 20u << '\t' << 20u << '\n';

  file.close();
}

size_t tst::getFileSize(const std::string &filename) {

    std::ifstream input(filename, std::ios::binary);
    assert(input.is_open());
    input.seekg(0, std::ios::end);
    return static_cast<size_t>(input.tellg());

}

bool tst::isFileEmpty(const std::string &filename) {

    return !static_cast<bool>(getFileSize(filename));

}

