#ifndef BRACHYPODE_TEST_UTILITIES_H
#define BRACHYPODE_TEST_UTILITIES_H

#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

namespace tst
{

  void makeValidLandscapeFile();

  std::vector<double> readfile(const std::string&);
  std::vector<size_t> readfile2(const std::string&);
  size_t getFileSize(const std::string&);
  bool isFileEmpty(const std::string&);

}

#endif

