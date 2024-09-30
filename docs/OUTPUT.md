## Output data

### Saving parameters

If `savepars` is `1` the program saves the parameters it has used to run the simulation in a parameter log file called `paramlog.txt`. These include the `seed` of the pseudo random number generator and any parameters that have been updated (e.g. `nloci` and `nchrom` if an architecture has been externally supplied, or `maxgrowths` if `type` has been set to `2`).

### Saving the genetic architecture

The genetic architecture will be saved into `architecture.txt` provided that parameter `savearch` is set to `1`.

### Screen output

The program outputs to the screen messages about the status of the simulation as well as the numbers of individuals in each deme at each generation (the latter only if `talkative` is `1`). If `savelog` is `1` the screen output of the program is redirected to a log file called `log.txt`. This may be useful if running simulations in batch on a remote server that does not save screen output, but still wanting to diagnose simulations after they are done.

### Saving data

The program saves data every `tsave` generations. The data are saved in binary format, in files with extension `.dat`. By default the program saves all of the following:
* `time.dat`: each saved generation
* `popsize.dat`: the total population size every saved generation
* `patchsizes.dat`: the number of individuals in each patch, in each deme, in each saved generation
* `traitmeans.dat`: the mean value of each trait (`x`, `y` and `z`) in each patch, in each deme, at each saved generation
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