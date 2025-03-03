In this guide we show how to build the program using CMake. We provide a ready-made `CMakeLists.txt` list of instructions but the user is free to use any custom configuration.

### Prerequisites

- Any modern C++ compiler (e.g. GCC, Clang or MGCV)
- Build system: [CMake](https://cmake.org/) (>= 3.16)
- Source retrieval: git (optional)

### Download the code

You can download the source code using `git`:

```shell
git clone https://github.com:rscherrer/brachypode
```

### Configure CMake

Our setup uses the following `CMakeLists.txt` configuration file:

```cmake
# CMakeLists.txt

# CMake
cmake_minimum_required(VERSION 3.16)

# Project name
project(brachypode)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

# Boilerplate
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Source code
add_subdirectory(src)
```

Make sure to save that file as `CMakeLists.txt` in the root directory of the repository.

### Build the program

Then, the following code sets the repository as working directory, creates a build folder and instructs CMake to build the program in release mode according to the instructions given in the `CMakeLists.txt` configuration file.

```shell 
cd brachypode
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

This setup places the compiled executable in the `bin/` folder.

(Note that the steps are the same on Windows, MacOS and Linux.)

### IDEs

Many IDEs such as Visual Studio or XCode support CMake out of the box. "Open folder" should do the trick...

You can use CMake to generate the input files for your favorite IDE too. For example, running the following from within the recently created `build/` folder will generate Visual Studio project files:

```shell
cmake -G "Visual Studio 17 2022" -A x64 ..
```

For XCode, run:

```shell
cmake -G Xcode    
```

This will place the project files in `build/`.
