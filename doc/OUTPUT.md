## Output Data

### Saving Parameters

If `savepars` is `1` the program saves the parameters it has used to run the simulation in a parameter log file called `paramlog.txt`. These include the `seed` of the pseudo random number generator and any parameters that have been updated (e.g. `nloci` if an architecture has been externally supplied). See [here](PARAMETERS.md) for details about parameters.

### Saving the Genetic Architecture

The genetic architecture will be saved into `architecture.txt` provided that parameter `savearch` is set to `1`. See [here](ARCHITECTURE.md) for details.

### Screen Output

The program outputs to the screen messages about the status of the simulation as well as the numbers of individuals in each deme at each generation (the latter only if `verbose` is `1`).

### Saving Data

The program saves data every `tsave` generations. The data are saved **in binary format**, in files with extension `.dat`. If data must be saved (`savedat 1`) and no variable choice is given (`choose 0`), by default the program saves all of the following:

| Name | Description | Frequency | 
|--|--|--|
| `time.dat` | Each saved generation | 1 per time step |
| `popsize.dat` | Total population size | 1 per time step |
| `patchsizes.dat` | Number of individuals in each patch | 2 per deme per time step |
| `traitmeans.dat` | Mean trait value in each patch | 2 per deme per time step |
| `individuals.dat` | Deme, patch and trait value of each individual | 3 per individual per time step |

To save a subset of those output data files, set `choose 1` in the parameter file, and provide a list of variables to save in a file called `whattosave.txt`, located in the working directory. This could be, for example, a file containing:

```
time
patchsizes
individuals
```

Note that the names of the variables to save must be given without the `.dat` extension. 

For the sake of speed and modularity in combining outputs into tables, the data are converted to **double precision floating point numbers** before being saved as **binary**, and because the data is saved in this format, each file contains a **one-dimensional array** of data (e.g. `individuals.dat` starts with the deme, patch, and trait value of the first individual of the first saved generation, followed by the same variables for the second individual, etc.). The data are therefore not human-readable. To read them, you must convert them back into numbers using **a function decoding binary**, and knowledge of **how many bytes each value takes**. Although double precision-floating point numbers usually take up **8 bytes** on a 64-bit system, this might vary, and so we recommend share the simulated data alongside platform details needed to read the data back in. To do just that and read the data into R, we developed the accompanying package [`brachypoder`](https://github.com/rscherrer/brachypoder).
