In this guide we show how to modify our [main CMake setup](SETUP.md) in order to test the program.

### Prerequisites

- Any modern C++ compiler (e.g. GCC, Clang or MGCV)
- Build system: [CMake](https://cmake.org/) (>= 3.16)
- Source retrieval: git

### Download the code 

First, download the repository and set it as working directory if you have not already done so:

```shell
git clone https://github.com/rscherrer/brachypode
cd brachypode
```

### Configure CMake

Make sure to save the following as a `CMakeLists.txt` file located in the root of the repository:

```cmake
# CMakeLists.txt

# CMake
cmake_minimum_required(VERSION 3.16)

# Set up vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
if (WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif()

# Project name
project(brachypode)

# Git will be needed to install Boost
find_package(Git REQUIRED)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --remote
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
if (NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

# Boilerplate
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Source code
add_subdirectory(src)
add_subdirectory(tests)
```

This file now contains all the build information to download the necessary tools from the Boost library and compile the tests using CMake. 

### Install Boost

Run the following to install the testing framework through `git`:

```shell
git submodule add --force https://github.com/microsoft/vcpkg
git submodule update --init --remote
```

These steps will connect the repo to Microsoft's [vcpkg](https://vcpkg.io/) installer, and set it up to download the required elements from the [Boost C++ library](https://www.boost.org/) necessary for unit testing (instructions for that are given in the file `vcpkg.json`). This setup has the advantage of integrating well with CMake and working well across platforms, so the steps are the same on Windows, Linux or MacOS. In case of errors during this installation process, please refer to the log files created by `vcpkg` (some dependencies may be missing).

### Build the tests

Then, run:

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

Here, the `cmake` command downloads the necessary dependencies through `vcpkg`, and builds all the targets (in debug mode) required by the `CMakeLists.txt` configuration --- not just the program itself, but also the tests, whose executables can be found in `bin/tests/`. Simply run those executables to run the tests. 
