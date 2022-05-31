# brachypode

This is the repository for a program simulating the evolution of an annual grass species in a semi-arid patchy environment.

## Build

The program was developed in C++20 on Ubuntu 20.04 and compiled using the CMake build system. The CMake files used during the development of the program are provided here.

### Linux, MacOS

First, CMake needs to be installed (this is the build system we use). To install CMake,

```bash
sudo apt install cmake		# replace 'apt' with your distro package manager
```

Then,

```bash
git clone git@github.com:rscherrer/brachypode.git
cd brachypode
git submodule add https://github.com/microsoft/vcpkg
git submodule update --init --remote
mkdir build && cd build
cmake ..
cmake --build .
```

This will build the executable `../bin/brachypode`.

Note that by doing this we clone the `vcpkg` repository from Microsoft into our repository. We use its utilities to be able to build our program on multiple platforms.

### Peregrine (University of Groningen High Performance Cluster)

```bash
module load CMake
module load binutils
module load git
```

```bash
git clone git@github.com:rscherrer/brachypode.git
cd brachypode
git submodule update --init --remote
mkdir build && cd build
cmake ..
cmake --build .
```

This will build the executable `../bin/brachypode`.

Here, it is important to make sure that the loaded version of CMake is of an adequate version compared to what the `CMakeLists.txt` file requires. You can explore the available versions on Peregrine by using `module spider CMake`, and replace `CMake` in `module load CMake` by the specific version that is required, if any error occurs. You can also change the required version of CMake in the `CMakeLists.txt` file to make sure it matches what is available on the cluster.

Check this [webpage](https://www.rug.nl/society-business/centre-for-information-technology/research/services/hpc/facilities/peregrine-hpc-cluster?lang=en) for more information about the Peregrine cluster.

### Windows

Download and install CMake at this link:

https://github.com/Kitware/CMake/releases/download/v3.23.0/cmake-3.23.0-windows-x86_64.msi

Make sure you select the option "Add CMake to the system PATH for the current user" when asked by the installer.

```bash
git clone git@github.com:rscherrer/brachypode.git
cd brachypode
git submodule add https://github.com/microsoft/vcpkg
git submodule update --init --remote
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

This will build the executable `../bin/brachypode.exe`.

### IDEs

Many IDEs (like VisualStudio, VisualCode, XCode, Qt(?)) support CMake out of the box. 'Open folder' should do the trick...

You can use CMake to generate the input files for your favoured IDE too:

```bash
git clone git@github.com:rscherrer/brachypode.git
cd brachypode
git submodule update --init --recursive
mkdir build
cd build
# Generate VisualStudio project files
cmake -G "Visual Studio 17 2022" -A x64 ..
# Generate Xcode project files (Xcode must be installed)
cmake -G Xcode    
```

This will place the project files in `../build`.

### Tests

The program was written using test-driven development. Tests of the different functionalities were performed using the Boost unit test framework. Source code for tests is available in the `tests/` folder. The CMake files provided with this repository build the test targets as well as the actual program. Hence, running the above steps (for any platform) should also build the executables for tests in `../bin/tests/`.

## Usage

This program is a command-line interface program. Assuming the name of the executable is `brachypode`, use, from the working directory:

```
./brachypode
```

to run a simulation with default parameters, and

```
./brachypode parameters.txt
```

where `parameters.txt` is a text file specifying custom parameters values to supply.

A parameter file (e.g. `parameters.txt`) should look like:


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

where the values were set to the default values of the model, except for `seed` whose default value is automatically generated within the program based on the time the simulation was launched.

The parameter file does not have to contain all the parameters in the model, just the ones that must be different from the default values. So, a parameter file containing, for example,

```
tend 100000
tsave 100
```

will work fine.

## Simulation details

### Parameter details

Note: hereafter "sites" refer to the demes and "patches" refer to facilitated and unfacilitated portions, respectively, within each site. Patch-specific parameters always take two values, the first one for the unfacilitated patch and the second one for the facilitated patch.

* `type`: the type of fitness function (`1` for type I -- the "quantitative genetics" model -- and `2` for type II -- the "ecophyiological tradeoff" model). In brief, in the type I model fitness depends only on trait `z`, while in the type II model fitness depends on the level of stress tolerance `x` and competitiveness `y`, both dependent on `z` and with a trade-off between the two.
* `popsize`: initial number of individuals
* `pgood`: the number of sites followed by the proportions of facilitated ("good") patches in each site, e.g. `3 0.5 0.4 0.1` for three sites with 50, 40 and 10% cover in facilitated patches, respectively
* `maxgrowths`: the maximum per capita growth rate in each patch. This corresponds to the basal number of offspring produced per individual per generation, before selection and density-dependence apply. When `type` is `2` there is only one basal growth rate, so only the first value of `maxgrowths` is used (the second is set to equal the first one).
* `stress`: the level of environmental stress in each patch. If `type` is `1`, then it refers to the point along phenotypic axis `z` that maximizes growth rate in the local patch (the "optimal" value of trait `z`). If `type` is `2`, then the growth rate of the individual depends on its stress tolerance level `x` relative to environmental stress (the growth rate is a logistic function decaying as stress goes up, with inflexion point located at `x`).
* `zwidths`: the niche width in each patch. This is the standard deviation of the Gaussian curve of growth rate as a function of trait `z`. Only applicable when `type` is `1`.
* `capacities`: the carrying capacity in each patch, in number of individuals per unit area. This is used to apply density-dependent population regulation. The growth rate of individuals is reduced, even more that the number of individuals locally competing approaches the carrying capacity. This is only used when `type` is `1`. When `type` is `2`, the realized growth rate of an individual depends on its competitive weight `y` relative to that of other individuals, and so the "carrying capacity" becomes emergent from the density and competitive weights of the individuals present. 
* `steep`: the intensity of the decay of the growth rate function when external stress is at its inflexion point (i.e. when external stress is equal to the level of stress tolerance of the individual, `x`). This corresponds to the absolute value of the (negative) slope of the curve at this point. Only applicable if `type` is `2`.
* `dispersal`: the rate of dispersal among sites, between zero and one. This is the number of individuals sampled each generation to migrate (migrants can end up in any other deme with equal probability).
* `mutation`: the mutation rate per locus, between zero and one (i.e. probability that a given allele flips to its opposite when an offspring is born).
* `nchrom`: the number of chromosomes.
* `nloci`: the number of loci coding for trait `z`.
* `effect`: the effect size of single loci, i.e. the effect on trait `z` when one allele mutates. This value cannot be negative.
* `allfreq`: the initial frequency of the 1-allele in the population.
* `xmax`: the maximum possible stress tolerance level `x`. This value is reached when all loci have allele `1`. Trait `x` cannot be negative.
* `ymax`: the maximum level of competitiveness `y`. This value is reached when all loci have allele `0`. Trait `y` cannot be negative.
* `tradeoff`: the trade-off between stress tolerance `x` and competitiveness `y`. Should be positive. The higher this trade-off, the least a given value of trait `z` can translate into both a high `x` and a high `y`.
* `selfing`: the proportion of offspring arising from selfing, between zero and one. Those offspring are clones of their parent plant. The rest of the offspring, produced through outcrossing, are produced by their mother plant but recombine with pollen randomly sampled from the entire population (this reflects the long-range dispersal ability of pollens).
* `recombination`: the recombination rate. This approximates the number of crossover events per individual per sexual reproduction event (not exactly its mean number though). Crossover events are randomly sampled along the genome, with distances exponentially distributed between consecutive crossovers. The recombination rate is the rate of this exponential distribution.
* `tend`: simulation time, in discrete generations.
* `tsave`: frequency with which to save the data, in number of generations.
* `seed`: the seed to use for the pseudo random number generator. If not supplied, the seed will be automatically set based on the exact time the simulation was launched. 
* `sow`: whether or not (`1` or `0`, respectively) the founder individuals should be randomly scattered throughout demes and patches at the start of the simulation
* `loadarch`: whether or not to load the genetic architecture from an existing file.
* `savepars`: whether or not to save the parameters used in the simulation (including the automatically generated seed if applicable)
* `savelog`: whether or not to redirect the screen output of the program to a file instead
* `savearch`: whether or not to save the genetic architecture used in the simulation (if `loadarch` is `1` the architecture file will simply be overwritten with itself)
* `talkative`: whether or not to output to screen (or file if `savelog` is `1`) the numbers of individuals in each site at every generation
* `choose`: whether or not to choose a subset of data to save

### Genetic architecture

The genetic architecture refers to the parameters controlling aspects of the genome and of the genotype-phenotype map. Because these parameters take one value per genetic unit (e.g. chromosome or locus) they may take many values, and so the program offers the possibility to sample them from some distributions whose hyperparameters are supplied in the parameter file. After being generated, the genetic architecture is saved in an architecture file called `architecture.txt`, in the working directory (if `savearch` is `1`). The program can also read a custom genetic architecture file, which must be in the same format as the file normally saved by the simulation. To do that, the name of the custom architecture file must also be `architecture.txt` and `loadarch` must be `1`.

In the program, the genetic architecture consists of two elements:
* `chromends`: the vector of genomic locations of each chromosome end.
* `locations`: the vector of genomic locations of each locus.

Hence, the hyperparameters used to generated those are `nchrom` and `nloci`. Genomic locations are continuous and between zero and one. If the architecture is supplied in a file, both chromosome ends and locus locations must be provided in increasing order, and the last chromosome end must always be one. (This matters for the recombinational process.)

The file `architecture.txt` may look like:

```
3 0.33 0.67 1
9 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 
```

where the first row contains the values for `chromends` and the second row for `locations`. Each row must start with the number of values to read for each vector (here, 3 chromosome ends and 9 locus locations, meaning there are 3 chromosomes and 9 loci).

After a custom architecture is read, its hyperparameters `nchrom` and `nloci` are updated within the program and in the list of parameters saved (if `savepars` is `1`).

Note that supplied genetic architectures need not be generated by hand but could be architectures generated in previous simulations.

### Saving parameters

If `savepars` is `1` the program saves the parameters it has used to run the simulation in a parameter log file called `paramlog.txt`. These include the `seed` of the pseudo random number generator and any parameters that have been updated (e.g. `nloci` and `nchrom` if an architecture has been externally supplied, or `maxgrowths` if `type` has been set to `2`).

### Screen output

The program outputs to the screen messages about the status of the simulation as well as the numbers of individuals in each deme at each generation (the latter only if `talkative` is `1`). If `savelog` is `1` the screen output of the program is redirected to a log file called `log.txt`. This may be useful if running simulations in batch on a remote server that does not save screen output, but still wanting to diagnose simulations after they are done.

### Saving data

The program saves data every `tsave` generations. The data are saved in binary format, in files with extension `.dat`. By default the program saves all of the following:
* `time.dat`: each saved generation
* `popsize.dat`: the total population size every saved generation
* `patchsizes.dat`: the number of individuals in each patch, in each deme, in each saved generation
* `traitmeants.dat`: the mean value of each trait (`x`, `y` and `z`) in each patch, in each deme, at each saved generation
* `individuals.dat`: the deme, patch, `x`, `y` and `z` values of each individual in each saved generation

The data are converted to double-precision numbers before being saved as binary, and because the data is saved in this format each file contains a one-dimensional array of data (e.g. `individuals.dat` starts with the deme, patch, `x`, `y` and `z` values of the first individual of the first saved generation, followed by the same variables for the second individual, etc.).

To save a subset of those output data files, set `choose` to `1` and provide a list of variables to save in a file called `whattosave.txt`, located in the working directory. For example, a file containing:

```
time
patchsizes
individuals
```

will only save the requested data files (do not specify any file extension).

The reason behind saving the data as binary arrays is that they are fast to write, fast to read and relatively light in terms of space. In addition, because not every variable may need to be saved depending on the simulation, we opted for saving the different variables in different files. This also makes it possible to assemble and organize the various saved data in different ways depending on the analysis that needs to be done.

## Analysis of the results

We developed an R package called [brachypoder](https://github.com/rscherrer/brachypoder) to perform these tasks and read the data into a format suitable for analysis in R.

## Content of this repository

* `main.cpp`: is the main source file of the program
* `src`: contains the source code (`.h` and `.cpp` files) for the program, coded in C++17
* `tests`: contains the testing scripts (the Boost C++ library was used for test-driven development)

## About the study

This repository only contains the simulation program used in our study. Content for the full study can be accessed at the following links (presently private):
* [data](https://github.com/rscherrer/brachypode-data) for the simulation data
* [results](https://github.com/rscherrer/brachypode-results) for scripts and figures
* [ms](https://github.com/rscherrer/brachypode-ms) for the manuscript

For full reproducibility of the analyses we recommend that the aforementioned repositories be downloaded within the current repository and with names as shown above.
