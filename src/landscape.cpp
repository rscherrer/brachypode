#include "landscape.h"

Landscape::Landscape(const size_t &n) :
    areas(std::vector<double>(n)),
    capacities(std::vector<size_t>(n)),
    habitats(std::vector<size_t>(n)),
    stresses(std::vector<double>(n)),
    competitions(std::vector<double>(n))
{

    // Unfacilitated matrix
    areas[0u] = 4.0;
    capacities[0u] = 50u;
    habitats[0u] = 0u;
    stresses[0u] = 5.0;
    competitions[0u] = 0.0;

    // Facilitated patches
    for (size_t i = 1u; i < n; ++i) {
        areas[i] = 1.0;
        capacities[i] = 20u;
        habitats[i] = 1u;
        stresses[i] = 0.0;
        competitions[i] = 0.1;
    }

}

// Load landscape parameters from file
void Landscape::load()
{

    const std::string filename = "landscape.txt";

    // Open the file
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename + '\n');

    // Read in the number of patches
    size_t n;
    file >> n;

    // Reset patch areas and capacities
    areas = std::vector<double>(n, 0.0);
    capacities = std::vector<size_t>(n, 0u);
    habitats = std::vector<size_t>(n, 0u);
    stresses = std::vector<double>(n, 0.0);
    competitions = std::vector<double>(n, 0.0);

    // Read them in back again from the file
    for (size_t i = 0u; i < n; ++i) file >> areas[i];
    for (size_t i = 0u; i < n; ++i) file >> capacities[i];
    for (size_t i = 0u; i < n; ++i) file >> habitats[i];
    for (size_t i = 0u; i < n; ++i) file >> stresses[i];
    for (size_t i = 0u; i < n; ++i) file >> competitions[i];

    // Check dimensions
    assert(areas.size() == n);
    assert(capacities.size() == n);
    assert(habitats.size() == n);
    assert(stresses.size() == n);
    assert(competitions.size() == n);

    // Close the file
    file.close();

}

void Landscape::setArea(const size_t &i, const double &x) { areas[i] = x; }
void Landscape::setCapacity(const size_t &i, const size_t &x) { capacities[i] = x; }
void Landscape::setHabitat(const size_t &i, const size_t &x) { habitats[i] = x; }
void Landscape::setStress(const size_t &i, const double &x) { stresses[i] = x; }
void Landscape::setCompetition(const size_t &i, const double &x) { competitions[i] = x; }

size_t Landscape::getNPatches() const { return areas.size(); }
double Landscape::getArea(const size_t &i) const { return areas[i]; }
size_t Landscape::getCapacity(const size_t &i) const { return capacities[i]; }
size_t Landscape::getHabitat(const size_t &i) const { return habitats[i]; }
double Landscape::getStress(const size_t &i) const { return stresses[i]; }
double Landscape::getCompetition(const size_t &i) const { return competitions[i]; }
