#include "library/Population.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(reproductionIncreasesPopSize) {

    Population pop(10u);
    const size_t sizeBefore = pop.getSize();
    pop.reproduce(3.0);
    BOOST_CHECK(pop.getSize() > sizeBefore);

}
