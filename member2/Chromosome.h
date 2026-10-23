#pragma once
#include <vector>
#include <cstdlib>
using namespace std;

// Base class: every chromosome has a list of genes and a fitness value.
template <class T>
class Chromosome {
public:
    vector<T> genes;
    double fitness;

    Chromosome() { fitness = 0; }

    int size() const { return genes.size(); }
};

// ---------------------------------------------------------------
// Binary chromosome: every gene is 0 or 1
// ---------------------------------------------------------------
class BinaryChromosome : public Chromosome<int> {
public:
    BinaryChromosome() {}

    // random chromosome
    BinaryChromosome(int length) {
        for (int i = 0; i < length; i++)
            genes.push_back(rand() % 2);
    }

    int randomGene() const { return rand() % 2; }
};

// ---------------------------------------------------------------
// Integer chromosome: every gene is an int between low and high.
// If isPermutation is true, the genes are the numbers low..high
// in some order (each number appears exactly once).
// ---------------------------------------------------------------
class IntegerChromosome : public Chromosome<int> {
public:
    int low, high;
    bool isPermutation;

    IntegerChromosome() { low = 0; high = 0; isPermutation = false; }

    IntegerChromosome(vector<int> g, int low, int high, bool isPermutation) {
        genes = g;
        this->low = low;
        this->high = high;
        this->isPermutation = isPermutation;
    }

    // random genes, each one between low and high
    static IntegerChromosome randomValues(int length, int low, int high) {
        vector<int> g;
        for (int i = 0; i < length; i++)
            g.push_back(low + rand() % (high - low + 1));
        return IntegerChromosome(g, low, high, false);
    }

    // random order of the numbers low..high (shuffle)
    static IntegerChromosome randomPermutation(int low, int high) {
        vector<int> g;
        for (int v = low; v <= high; v++)
            g.push_back(v);

        for (int i = g.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = g[i];
            g[i] = g[j];
            g[j] = temp;
        }
        return IntegerChromosome(g, low, high, true);
    }

    int randomGene() const { return low + rand() % (high - low + 1); }
};

// ---------------------------------------------------------------
// Floating-point chromosome: every gene is a double between low and high
// ---------------------------------------------------------------
class FloatChromosome : public Chromosome<double> {
public:
    double low, high;

    FloatChromosome() { low = 0; high = 1; }

    FloatChromosome(vector<double> g, double low, double high) {
        genes = g;
        this->low = low;
        this->high = high;
    }

    // random chromosome
    FloatChromosome(int length, double low, double high) {
        this->low = low;
        this->high = high;
        for (int i = 0; i < length; i++)
            genes.push_back(randomGene());
    }

    double randomGene() const {
        double r = (double)rand() / RAND_MAX;   // number between 0 and 1
        return low + r * (high - low);
    }
};
