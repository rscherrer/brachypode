## Development tools

The specific scripts and files used during the development of the program can be found in the `dev/` folder. 

### Configurations

The `dev/` folder contains a subfolder called `cmake/`, in which we store the CMake configuration files used for various tasks, including:

* `Release.txt` to compile in release mode for the end user (default)
* `Tests.txt` to compile the tests in debug mode and check for memory leaks
* `Profile.txt` to profile the code in release mode 
* `Coverage.txt` to analyze code coverage across tests

To use any of these build configurations, simply copy their content in the `CMakeLists.txt` file located in the root folder, and build the project with CMake (as shown [here](../doc/SETUP.md) and [here](../doc/TESTS.md)). The `CMakesLists.txt` files located in the `src/` and the `tests/` directories should not be changed.

### Helpers

In addition, the `dev/` folder contains custom helper scripts used to automatize the quality checks described above:

* `run_tests.sh` runs all the tests and checks for errors
* `run_valgrind.sh` runs all the tests while analysing memory use
* `run_lcov.sh` runs all the tests and analyzed coverage
* `run_gprof.sh` runs the main program and analyzes performance

(Use `chmod +x ...` if needed to allow these scripts to run.) 

These scripts must be run from the root directory after the relevant executables have been built. Specifically, `run_tests.sh` and `run_valgrind.sh` require the test executables (e.g. configurations `Tests.txt` or `Coverage.txt`), while `run_lcov.sh` requires tests with coverage enabled (e.g. `Coverage.txt`) and `run_gprof.sh` requires the compiled program with profiling flags on (e.g. `Profile.txt`).

Please note that these scripts are helper tools used on a Linux machine during development. As such, they **are not made to be compatible across platforms** and will require specific packages installed in order to run (e.g. Valgrind or LCOV).