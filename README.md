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
- Workout inheritance
- Workout the scales that the parameters should have relative to the traits --- user supplied or automatic rescaling?
- Write a test for extinction use case
- Does computation time not depend heavily on the size of the individual bitset?
- Could populations, landscapes and genetic architectures not store relevant parameters in them so they do not rely on further supply of parameters from the parameter structure?
- Pass parameters to the landscape