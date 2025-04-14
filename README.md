# brachypode

This is the repository for a program simulating the evolution of an annual grass species in a semi-arid patchy environment.

## Description

The model is an agent-based simulation where sessile individuals dwel in a metapopulation consisting of multiple demes, and each deme contains a facilitated and an unfactilitated habitat patch (facilitated patches are basically the undergrowth of nurse shrubs). The individuals possess genes that determine some level of tolerance against the harsh conditions encountered outside of the shrubs, which trade off with reproductive output.

## Installation

This program must be compiled. It is written in C++ and can be built using any modern C++ compiler and build system. [Here](doc/SETUP.md) we provide an example setup to build the program on several platforms using CMake.

## Usage

This program has a command-line interface. Assuming the name of the compiled executable is `brachypode`, use the following command in the terminal from the working directory to run a simulation with default parameter values:

```shell
./brachypode
```

To run a simulation with custom parameters, use:

```shell
./brachypode parameters.txt
```

where `parameters.txt` is a text file containing a list of parameter names and values.

### Parameters

The parameter file should look like this:

```
popsize 10
pgood 3 0.8 0.6 0.5
pgoodEnd 0.8 0.6 0.5
stress 4 0
stressEnd 4 0
capacities 100 1000
capacities 100 1000
maxgrowth 2
steep 2
dispersal 0.1
mutation 0.0001
nloci 50
effect 0.1
allfreq 0.5
tradeoff 1
selfing 0.95
recombination 1
tend 100
tsave 20
seed 12345
sow 1
loadarch 0
savepars 1
savelog 0
savearch 1
choose 0
```

In the parameter file, each line should contain a parameter name followed by its value(s). If there are several values, they should be separated by spaces. Invalid parameter names or values will result in an error, and lines starting with **#** will be treated as comments. Click [here](doc/PARAMETERS.md) for a description of the available parameters and their meaning.

### Genetic architecture

The genetic architecture refers to parameters of the genotype-phenotype map, which are on a per locus basis and may take a lot of space, especially if there are many loci. 

Providing a genetic architecture is not necessary, as it will by default be generated anew based on genetic hyperparameters given in the parameter file such as `nloci` or `effect`. However, it is possible to supply a custom genetic architecture by providing an architecture file, as explained [here](doc/ARCHITECTURE).

### Output

The user can choose which variables to save from the simulation. Those have to be encoded in the `whattosave.txt` file provided that parameter `choose` is set to `1` within the `parameters.txt` file, as explained [here](doc/OUTPUT.md). Output data are saved into binary files with the `.dat` extension, which can be read by the custom-made [R](https://www.r-project.org/) package [brachypoder](https://github.com/rscherrer/brachypoder).

## Tests

This program was tested using the [Boost.Test](https://www.boost.org/doc/libs/1_85_0/libs/test/doc/html/index.html) library. All the tests can be found in the `tests/` folder. [Here](doc/TESTS.md) we show how we tested the program locally using our own custom setup.

## About

This code was written in **C++20**, making mostly use of [Visual Studio Code](https://code.visualstudio.com/) (v1.99.0), [CMake](https://cmake.org/) (v3.28.3) and [GCC's g++](https://gcc.gnu.org/) (v13.3.0) on **Linux Ubuntu 24.04 LTS**. 

**Tests** (see [here](doc/TESTS.md)) were written with [Boost.Test](https://www.boost.org/doc/libs/1_85_0/libs/test/doc/html/index.html) (v1.87), which was retrieved with [Git](https://git-scm.com/) (v2.43.0) and [vcpkg](https://github.com/microsoft/vcpkg) (v2025.04.09). **Memory use** was checked with [Valgrind](https://valgrind.org/) (v3.22.0). **Code coverage** was analyzed with [gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) (v13.3.0) and [LCOV](https://github.com/linux-test-project/lcov) (v2.0-1) and [Coverage Gutters](https://github.com/ryanluker/vscode-coverage-gutters) (v2.13.0). **Profiling** was performed with [gprof](https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_mono/gprof.html) (v2.42). (See [this page](dev/README.md) for details.)

During development, occasional use was also made of **ChatGPT** and **GitHub Copilot**.

## Links

* [brachypoder](https://github.com/rscherrer/brachypoder): package to read the simulations in R
* [brachypode-results](https://github.com/rscherrer/brachypode-results): generated data and analyses of the results
* [brachypode-ms](https://github.com/rscherrer/brachypode-ms): manuscript
* [brachypode-nb](https://github.com/rscherrer/brachypode-approx): Mathematica notebooks used for some derivations

## Permissions

Copyright (c) [Raphaël Scherrer](https://github.com/rscherrer), 2024 (open source license will be added upon publication). This code comes with no guarantee whatsoever.
