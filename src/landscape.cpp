#include "landscape.h"

Landscape::Landscape(const Param &p) :
    npatches(p.nfacil + 1u),
    areas(std::vector<double>(npatches)),
    capacities(std::vector<size_t>(npatches)),
    habitats(std::vector<size_t>(npatches)),
    stresses(std::vector<double>(npatches)),
    competitions(std::vector<double>(npatches))
{

    // Unfacilitated matrix
    areas[0u] = p.area[0u];
    capacities[0u] = p.capacity[0u];
    habitats[0u] = 0u;
    stresses[0u] = p.stress[0u];
    competitions[0u] = 0.0; // this value is never used

    // Facilitated patches
    for (size_t i = 1u; i < npatches; ++i) {

        areas[i] = p.area[1u];
        capacities[i] = p.capacity[1u];
        habitats[i] = 1u;
        stresses[i] = p.stress[1u];
        competitions[i] = p.competition;

    }

    assert(areas.size() == npatches);
    assert(capacities.size() == npatches);
    assert(habitats.size() == npatches);
    assert(stresses.size() == npatches);
    assert(competitions.size() == npatches);

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
