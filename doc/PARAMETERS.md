## List of parameters

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