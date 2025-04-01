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

# Debug mode
set(CMAKE_BUILD_TYPE "Coverage")

# Set up code coverage
message(STATUS "Building with code coverage enabled")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Debugging messages for flags
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

# Source code
add_subdirectory(src)
add_subdirectory(tests)




And then after building, ./generate_coverage.sh:

#!/bin/bash

# Clean existing coverage data
find . -name "*.gcda" -exec rm {} +

# Run all test executables
./bin/tests/buffer_tests
./bin/tests/individual_tests
./bin/tests/parameters_tests
./bin/tests/architecture_tests
./bin/tests/printer_tests
./bin/tests/population_tests
./bin/tests/utilities_tests
./bin/tests/tests

# Capture coverage data
lcov --capture --directory build/ --output-file lcov.info --ignore-errors mismatch

# Filter out unnecessary files
lcov --remove lcov.info '/usr/*' 'tests/*' 'build/*/boost/*' --output-file lcov.info

# Generate HTML report
genhtml lcov.info --output-directory lcov

# Open the report
xdg-open lcov/index.html