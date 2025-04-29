#!/bin/bash

## This script builds the program according to the instructions
## found in the CMakeLists.txt configuration file. To be run
## from the root directory of the project.

# Home directory
HOMEDIR=$(pwd)

# Remove build and bin
rm -rf build
rm -rf bin

# Make a build directory
mkdir -p build

# Go there
cd build

# Prepare
cmake ..

# Build
cmake --build .

# Back to home
cd $HOMEDIR