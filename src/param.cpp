#include "param.h"

Param::Param() :
    popsize(100u),
    nfacil(10u),
    maxgrowth(2.0),
    steepness(10.0),
    costcomp(0.02),
    costtol(0.02),
    tradeoff(0.01),
    nchrom(1u),
    ncomp(1u),
    ntol(1u),
    nneut(1u),
    allfreq(0.2),
    mutation(0.0001),
    seed(makeDefaultSeed()),
    savepars(true),
    loadlandscape(false),
    loadarch(false)
{

    // Make sure parameter values make sense
    check();

    // Seed the random number generator
    rnd::rng.seed(seed);
}

// Create a default seed based on clock
size_t Param::makeDefaultSeed()
{
    return static_cast<size_t>(std::chrono::high_resolution_clock::now().
     time_since_epoch().count());
}

// Read parameters from a file
void Param::read(const std::string &filename)
{
    std::ifstream inputfile;
    inputfile.open(filename);
    if (!inputfile.is_open()) {
        std::string msg = "Unable to open parameter file ";
        throw std::runtime_error(msg + filename);
    }

    import(inputfile);
    inputfile.close();
}

// Import parameters from a file stream
void Param::import(std::ifstream &file)
{

    std::string input;

    while (file >> input) {

        if (input == "popsize") file >> popsize;
        else if (input == "nfacil") file >> nfacil;
        else if (input == "maxgrowth") file >> maxgrowth;
        else if (input == "steepness") file >> steepness;
        else if (input == "costcomp") file >> costcomp;
        else if (input == "costtol") file >> costtol;
        else if (input == "tradeoff") file >> tradeoff;
        else if (input == "nchrom") file >> nchrom;
        else if (input == "ncomp") file >> ncomp;
        else if (input == "ntol") file >> ntol;
        else if (input == "nneut") file >> nneut;
        else if (input == "allfreq") file >> allfreq;
        else if (input == "mutation") file >> mutation;
        else if (input == "seed") file >> seed;
        else if (input == "savepars") file >> savepars;
        else if (input == "loadlandscape") file >> loadlandscape;
        else if (input == "loadarch") file >> loadarch;
        else
            throw std::runtime_error("Invalid parameter name: " + input);

    }

    // Now update interactive parameters
    update();

    std::clog << "Parameters were read in succesfully.\n";

}

// Update the system after having read new parameters
void Param::update()
{
    rnd::rng.seed(seed);
    check();
}

// Check that the parameter values are valid
void Param::check() const
{
    std::string msg = "No error detected";

    if (maxgrowth < 0.0) msg = "Maximum growth rate should be positive";
    if (steepness < 0.0) msg = "Steepness of fitness decay should be positive";
    if (costcomp < 0.0) msg = "Cost of competitiveness should be positive";
    if (costtol < 0.0) msg = "Cost of tolerance should be positive";
    if (tradeoff < 0.0) msg = "Trade-off should be positive";
    if (nchrom < 1u) msg = "There should be at least one chromosome";
    if (ncomp < 1u) msg = "There should be at least one competitiveness locus";
    if (ntol < 1u) msg = "There should be at least one tolerance locus";
    if (nneut < 1u) msg = "There should be at least one neutral locus";
    if (allfreq < 0.0 || allfreq > 1.0) msg = "Initial allele frequency should be between zero and one";
    if (mutation < 0.0 || mutation > 1.0) msg = "Mutation rate should be between zero and one";

    if (msg != "No error detected")
        throw std::runtime_error(msg);
}

// Save parameters to a file
void Param::save() const
{
    const std::string filename = "paramlog.txt";
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);
    write(file);
    file.close();
}

// Write parameters to a file stream
void Param::write(std::ofstream &file) const
{

    file << "popsize " << popsize << '\n';
    file << "nfacil " << nfacil << '\n';
    file << "maxgrowth " << maxgrowth << '\n';
    file << "steepness " << steepness << '\n';
    file << "costcomp " << costcomp << '\n';
    file << "costtol " << costtol << '\n';
    file << "tradeoff " << tradeoff << '\n';
    file << "nchrom" << nchrom << '\n';
    file << "ncomp " << ncomp << '\n';
    file << "ntol " << ntol << '\n';
    file << "nneut " << nneut << '\n';
    file << "allfreq " << allfreq << '\n';
    file << "mutation " << mutation << '\n';
    file << "seed " << seed << '\n';
    file << "savepars " << savepars << '\n';
    file << "loadlandscape " << loadlandscape << '\n';
    file << "loadarch " << loadarch << '\n';

}
