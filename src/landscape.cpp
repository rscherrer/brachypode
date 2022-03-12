#include "landscape.h"

Landscape::Landscape(const Parameters &p) :
    npatches(p.npatches),
    areas(std::vector<double>(npatches)),
    capacities(std::vector<double>(npatches)),
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

    std::string field;

    file >> field;
    if (field == "npatches") file >> npatches;
    else throw std::runtime_error("Number of patches should be provided first");

    areas = std::vector<double>(npatches);
    capacities = std::vector<double>(npatches);
    habitats = std::vector<size_t>(npatches);
    stresses = std::vector<double>(npatches);
    competitions = std::vector<double>(npatches);

    bool isAreas = false, isCapacities = false, isHabitats = false;
    bool isStresses = false, isCompetitions = false;

    while (file >> field) {

        if (field == "areas") { isAreas = true; for (size_t j = 0u; j < npatches; ++j) file >> areas[j]; }
        else if (field == "capacities") { isCapacities = true; for (size_t j = 0u; j < npatches; ++j) file >> capacities[j]; }
        else if (field == "habitats") { isHabitats = true; for (size_t j = 0u; j < npatches; ++j) file >> habitats[j]; }
        else if (field == "stresses") { isStresses = true; for (size_t j = 0u; j < npatches; ++j) file >> stresses[j]; }
        else if (field == "competitions") { isCompetitions = true; for (size_t j = 0u; j < npatches; ++j) file >> competitions[j]; }
        else throw std::runtime_error("Unknown landscape field provided");

    }

    file.close();

    if (!(isAreas && isCapacities && isHabitats && isStresses && isCompetitions))
        throw std::runtime_error("Some landscape fields are missing");

    assert(areas.size() == npatches);
    assert(capacities.size() == npatches);
    assert(habitats.size() == npatches);
    assert(stresses.size() == npatches);
    assert(competitions.size() == npatches);

    // For each patch...
    for (size_t j = 0u; j < npatches; ++j) {

        if (areas[j] <= 0.0) throw std::runtime_error("Patches should have positive areas");
        if (capacities[j] <= 0.0) throw std::runtime_error("Patches should have positive carrying capacities");
        if (habitats[j] > 1u) throw std::runtime_error("Habitats can only be zero or one");
        if (stresses[j] < 0.0) throw std::runtime_error("Stress should be positive");
        if (competitions[j] < 0.0) throw std::runtime_error("Competition should be positive");

    }
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
