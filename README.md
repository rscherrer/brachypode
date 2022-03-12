# brachypode

Simulating the eco-evolutionary dynamics of life under facilitation in Brachypodium plant populations across semi-arid environments from Southern Spain

Megan Korte and Raphael Scherrer

Branch|[![Travis CI logo](ci/pics/TravisCI.png)](https://travis-ci.com)|[![Codecov logo](ci/pics/Codecov.png)](https://www.codecov.io)
---|---|---
master|[![Build Status](https://travis-ci.com/rscherrer/brachypode.svg?branch=master)](https://travis-ci.com/rscherrer/brachypode)|[![codecov.io](https://codecov.io/github/rscherrer/brachypode/coverage.svg?branch=master)](https://codecov.io/github/rscherrer/brachypode/branch/master)
develop|[![Build Status](https://travis-ci.com/rscherrer/brachypode.svg?branch=develop)](https://travis-ci.com/rscherrer/brachypode)|[![codecov.io](https://codecov.io/github/rscherrer/brachypode/coverage.svg?branch=develop)](https://codecov.io/github/rscherrer/brachypode/branch/develop)
raph|[![Build Status](https://travis-ci.com/rscherrer/brachypode.svg?branch=raph)](https://travis-ci.com/rscherrer/brachypode)|[![codecov.io](https://codecov.io/github/rscherrer/brachypode/coverage.svg?branch=raph)](https://codecov.io/github/rscherrer/brachypode/branch/raph)
megan|[![Build Status](https://travis-ci.com/rscherrer/brachypode.svg?branch=megan)](https://travis-ci.com/rscherrer/brachypode)|[![codecov.io](https://codecov.io/github/rscherrer/brachypode/coverage.svg?branch=megan)](https://codecov.io/github/rscherrer/brachypode/branch/megan)
richel|[![Build Status](https://travis-ci.com/rscherrer/brachypode.svg?branch=richel)](https://travis-ci.com/rscherrer/brachypode)|[![codecov.io](https://codecov.io/github/rscherrer/brachypode/coverage.svg?branch=richel)](https://codecov.io/github/rscherrer/brachypode/branch/richel)


Features to add:
- Maybe patches should be their own class?
- maxdensities may not be the best name, risk to confuse it with capacities
- the way we disperse seeds now is wonky: we impose a cap on the number of seeds depending on what each patch can host but then we distribute the seeds at random across the patches, so some patches will sometimes have slightly more than they can support (prob. not a big problem)
- Workout the scales that the parameters should have relative to the traits --- user supplied or automatic rescaling?
- Does computation time not depend heavily on the size of the individual bitset?
- Could populations, landscapes and genetic architectures not store relevant parameters in them so they do not rely on further supply of parameters from the parameter structure?
- Change the order of ncomp and ntol in parameters
- Do we pass values instead of classes (architecture and landscape)? makes sense if not too many parameters to pass
- Do classes inherit hyperparameters? makes sense if we want to avoid passing both classes and parameters to some functions
- The simplest way to externally update the classes is to supply in the file exactly the hyperparameters and the parameters, then the class should contain fields for its hyperparameters as well
- Classes should have parameters as well as hyperparameters
- Files providing these parameters should have all of the corresponding fields, named
- Prefer j over i as iterator for patches over the landscape
- Check for missing fields in architecture
- Resize architecture by resetting all the fields
- Can locations be negative?
- Chromosomes should be guessed, not supplied
- Better implement test files
