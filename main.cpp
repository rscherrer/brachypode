#include <iostream>
#include <vector>

// This is the individual class
// All attributes of individuals must be introduced here
// We are not creating an individual here, just providing a template
// to build one
// This bit will be moved to a separate header file, but it is fine to keep
// it here for now.

class Individual {

public:

    // Public stuff can be called from outside
    // Can be functions that access private stuff for example:

    size_t getSize() { return size; }
    bool isSheltered() { return sheltered; }

private:

    // Private stuff can only be modified through member functions of the class,
    // that is, functions belonging to the class. No third party. This is
    // for security purposes.

    size_t size;
    bool sheltered;

};

int main() {

    // Create a population of 10 individuals
    std::vector<Individual> population(10u);

    // Loop through generations
    for (size_t t = 0u; t < 100u; ++t) {

        // Nothing really happens for now
        std::cout << "Generation " << t << ", " <<
         population.size() << " individuals\n";

    }

}
