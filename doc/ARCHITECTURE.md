## Genetic Architecture

The genetic architecture refers to the parameters controlling aspects of the genome and of the **genotype-phenotype map**. Because these parameters take one value per genetic locus, they may be many, and so they are read and stored separately from the other parameters. 

The details of the genetic architecture can be supplied in a text file called `architecture.txt` if parameter `loadarch` is set to `1` (see [here](PARAMETERS.md) for how to set basic simulation parameters). To be read, this file must be in the working directory from which the program is run.

Please note that if not read (`loadarch 0`), the genetic architecture **will be generated anew** during the simulation (with default parameter values as shown below). To save a generated architecture to `architecture.txt`, set parameter `savearch` to `1`.

The file `architecture.txt` should look like this:

```
nloci 5
locations 0.1 0.2 0.5 0.6 0.99
effects 0.1 0.1 0.1 3.14 0.1 
```

where the first row gives the **number of loci** (a single number), while the other two give the **genomic locations** and **effect sizes** of each locus, respectively. This file is read in the same way as the [parameter file](PARAMETERS.md) and so each line should start with the name of the parameter to be read, and the values should be separated by spaces.

Some details about the parameters of the genetic architecture:

| Name | Default value(s) | Accepted values | Description | No. values | Notes |
|--|--|--|--|--|--|
| `nloci` | `10` | Strictly positive integers | The number of loci in the genetic architecture | 1 | Will override the `nloci` provided in the [parameter file](PARAMETERS.md), if any
| `locations` | Randomly generated from a uniform distribution | Decimals from zero to one | The relative genomic position of each locus | `nloci` | Must be provided in strictly increasing order and not contain duplicates 
| `effects` | All `0.1` | Strictly positive decimals | The additive contribution of a one-allele to the phenotype at each locus | `nloci` | Will all be equal to parameter `effect` (defaulting to `0.1`) if no architecture is provided (`loadarch 0`)

The architecture file should be provided when a **custom architecture is needed**, for example, one with varying effect sizes across loci (contrary to the default implementation which attributes the same effect size to all loci). 

The number of loci in the architecture file does not need to match that of the parameter file, as `nloci` will be overwritten when the architecture file is read in.