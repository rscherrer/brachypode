## List of parameters

Here we provide an explanation of the various parameters of the model (passed though a parameter file, e.g. `parameters.txt`), as well as their default value(s) when no parameter file is supplied, and their valid ranges. For more information, please refer to the source code or the scientific manuscript accompanying this project (link will be made available once published).

Not that hereafter, "positive" means ***including zero***, while "strictly positive" means ***excluding zero***. Also note that missing parameters in the parameter file will take their default value.

For parameters specific to the genet

| Parameter name | Default value(s) | Accepted values | Description | No. values | Notes |
|--|--|--|--|--|--|
| `popsize` | `10` | Positive integers | Number of individuals in the starting population | `1` |
| `ndemes` | `3` | Positive integers | Number of demes in the landscape | 1 | If set, make sure that `pgood` and `pgoodEnd` have the right number of values
| `pgood` | `0.8 0.8 0.8` | Decimals from zero to one | Proportion of the area covered by good patches in each deme | `ndemes` | If set, make sure that `ndemes` is set (or defaults) to read the right number of values
| `pgoodEnd` | `0.1 0.1 0.1` | Decimals from zero to one | Same as `pgood` but at the end of the period of climate change | `ndemes` | If set, make sure that `ndemes` is set (or defaults) to read the right number of values
| `stress` | `4 0` | Positive decimals | Amount of stress in the unfacilitated and facilitated patches, respectively | 2 |
| `stressEnd` | `4 0` | Positive decimals | Same as `stress` but at the end of the period of climate change | 2 |
| `capacities` | `100 10000` | Strictly positive decimals | Carrying capacity per unit area in unfacilated and facilitated patches, respectively | 2 |
| `capacitiesEnd` | `100 10000` | Strictly positive decimals | Same as `capacities` but at the end of the period of climate change | 2 |
| `maxgrowth` | `4` | Positive decimals | Maximum achievable intrinsic growth rate in a Ricker-type population model | 1 |
| `steep` | `2` | Positive decimals | Magnitude of the downward slope of the survival probability function at its inflection point | 1 |
| `dispersal` | `0.01` | Decimals from zero to one | Per capita probability of any individual to disperse to another (random) deme during the dispersal step | 1 |
| `mutation` | `0.0001` | Decimals from zero to one | Probability of any given locus to flip to its opposite allele during the mutation process | 1 |
| `nloci` | `10` | Strictly positive integers | Number of loci in the genome | 1 |
| `effect` | `0.1` | Strictly positive decimals | Additive contribution to the phenotype of the one-allele at any given locus | 1 | Equal across all loci if the genetic architecture is generated anew (`loadarch 0`), but can differ among loci if `architecture.txt` is provided (`loadarch 1`, see details [here](ARCHITECTURE.md)) |
| `allfreq` | `0.1` | Decimals from zero to one | Frequency of the one-allele in the starting population | 1 |
| `tradeoff` | `0.1` | Positive decimals | Magnitude of the negative linear effect of an increase in stress tolerance on the intrinsic reproductive output of any given individual | 1 |
| `nonlinear` | `1` | Strictly positive decimals | Degree of the polynomial describing the decrease of intrinsic reproductive output with increased stress tolerance | 1 | Below 1 for convex, above 1 for concave, 1 for linear curve |
| `selfing` | `0.95` | Decimals from zero to one | Probability of each offspring produced to be a clone of its parent plant (and not of sexual reproduction with a pollen donor) | 1 |
| `recombination` | `1` | Positive decimals | Average distance between consecutive crossovers during the recombination process, 1 being the length of the genome | 1 | Set 0 for no recombination 
| `precis` | `1e-06` | Strictly positive decimals | The minimum value that the realized carrying capacity can take within a patch | 1 | Should be very small
| `memsave` | `1` | Decimals greater than the size of a double precision floating point number (usually 8 bytes, i.e. 8e-06 MB, on a 64 bit system), in MB | Memory that can be filled in by each data saving buffer (in MB) before writing to file | 1 | There will be as many open buffers as there are output files to save (see details [here](OUTPUT.md))
| `tend` | `10` | Strictly positive integers | Number of simulation time steps | 1 | 
| `tsave` | `20` | Strictly positive integers | Frequency of data saving (in time steps) | 1 | There will be **`tend` / `tsave` + 1** time steps saved, including the first and last ones
| `tchange` | `100000` | Positive integers | Time at which climate change starts | 1 | Note that at `tchange`, climate change has not started yet. It only starts at the next generation.
| `twarming` | `1` | Strictly positive integers | Duration of the climate change period (in time steps) | 1 |
| `seed` | Clock-generated | Positive integers | Seed of the pseudo-random number generator | 1 | The clock is used to generate a pseudo-random seed. Make sure to set `savepars` to 1 to be able to retrieve the generated seed and reproduce a given simulation. | 
| `sow` | `0` | One or zero | Whether or not the individuals should be randomly distributed across demes at the start of the simulation | 1 |
| `loadarch` | `0` | One or zero | Whether the genetic architecture should be read from a file named `architecture.txt` instead of being generated anew | 1 | Check [here](ARCHITECTURE.md) for details
| `savepars` | `0` | One or zero | Whether or not to save the parameters into a parameter log file called `paramlog.txt` | 1 |
| `savearch` | `0` | One or zero | Whether or not to save the genetic architecture into a text file called `architecture.txt` | 1 |
| `savedat` | `0` | One or zero | Whether or not to save output data | 1 | Check [here](OUTPUT.md) for details
| `choose` | `0` | One or zero | Whether or not to choose which output variables to save by providing a `whattosave.txt` file | 1 | See [here](OUTPUT.md) for how this works |
| `verbose` | `0` | One or zero | Whether or not to display progress at each time step to the screen | 1 | |