# brachypode

This is the repository for a program simulating the evolution of an annual grass species in a semi-arid patchy environment.

## Description

This is an agent-based simulation where sessile individuals dwel in a metapopulation consisting of multiple demes, and each deme contains a facilitated and an unfactilitated habitat patch (facilitated patches are basically the undergrowth of nurse shrubs). The individuals possess genes that determine some level of tolerance against the harsh conditions encountered outside of the shrubs, which trade off with reproductive output. See manuscript for more details.

## Installation

This program is written in C++ and can be compiled using any modern C++ compiler and build system. [Here](doc/SETUP.md) we provide an example setup to build the program on several platforms using the build system CMake.

## Usage

This program is a command-line interface program. Assuming the name of the executable is `brachypode`, use the following command in the terminal from the working directory to run a simulation with default parameter values:

```shell
./brachypode
```

To run a simulation with custom parameters, use:

```shell
./brachypode parameters.txt
```

where `parameters.txt` is a text file containing a list of parameter names and values.

### Parameters

The file `parameters.txt` should look like:

```
type 1
popsize 10
pgood 3 0.8 0.6 0.5 0.3 0.1
maxgrowths 1 2
stress 4 0
zwidths 1 2
capacities 100 10000
steep 2
dispersal 0.1
mutation 0.0001
nchrom 1
nloci 50
effect 0.1
allfreq 0.5
xmax 5
ymax 5
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
talkative 0
choose 0
```

and should be present within the working directory where the simulation is launched. Click [here](doc/PARAMETERS.md) for a detailed description of the parameters and their meaning.

### Genetic architecture

For parameters that have to do with the genetic architecture of the evolving traits in this simulation, see [here](doc/ARCHITECTURE).

### Output

The user can choose which variables to save from the simulation. Those have to be encoded in the `whattosave.txt` file provided that parameter `choose` is set to `1` within the `parameters.txt` file, as explained [here](doc/OUTPUT.md). Output data are saved into binary files with the `.dat` extension, which can be read by the custom-made [R](https://www.r-project.org/) package [brachypoder](https://github.com/rscherrer/brachypoder).

## Tests

This program was tested using the Boost Test library. All the tests can be found in the `tests/` folder. [Here](doc/TESTS.md) we show how we tested the program locally using our own custom setup.

## Links

* [brachypoder](https://github.com/rscherrer/brachypoder): package to read the simulations in R
* [brachypode-results](https://github.com/rscherrer/brachypode-results): generated data and analyses of the results
* [brachypode-ms](https://github.com/rscherrer/brachypode-ms): manuscript
* [brachypode-nb](https://github.com/rscherrer/brachypode-approx): Mathematica notebooks used for some derivations

## Permissions

Copyright (c) Raphaël Scherrer, 2024 (open source license will be added upon publication). This code comes with no guarantee whatsoever.
