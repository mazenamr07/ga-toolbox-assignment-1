#include "ga/GAEngine.h"
#include "CVRP.h"
#include <random>
#include <algorithm>
#include <vector>

namespace ga {

class BitFlipMutation : public Mutation {
public:
    double mutationRate;
    explicit BitFlipMutation(double rate = 0.05) : mutationRate(rate) {}

    void mutate(Chromosome& chromosome, std::mt19937& rng) override {
        auto& bin = dynamic_cast<BinaryChromosome&>(chromosome);
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        for (auto& gene : bin.genes) {
            if (dist(rng) < mutationRate) {
                gene = 1 - gene;
            }
        }
    }
};

class SwapMutation : public Mutation {
public:
    double mutationRate;
    explicit SwapMutation(double rate = 0.1) : mutationRate(rate) {}

    void mutate(Chromosome& chromosome, std::mt19937& rng) override {
        auto& integerChr = dynamic_cast<IntegerChromosome&>(chromosome);
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        
        if (dist(rng) < mutationRate && integerChr.genes.size() > 1) {
            std::uniform_int_distribution<size_t> indexDist(0, integerChr.genes.size() - 1);
            size_t idx1 = indexDist(rng);
            size_t idx2 = indexDist(rng);
            while (idx1 == idx2) {
                idx2 = indexDist(rng);
            }
            std::swap(integerChr.genes[idx1], integerChr.genes[idx2]);
        }
    }
};

class InversionMutation : public Mutation {
public:
    double mutationRate;
    explicit InversionMutation(double rate = 0.1) : mutationRate(rate) {}

    void mutate(Chromosome& chromosome, std::mt19937& rng) override {
        auto& integerChr = dynamic_cast<IntegerChromosome&>(chromosome);
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        
        if (dist(rng) < mutationRate && integerChr.genes.size() > 2) {
            std::uniform_int_distribution<size_t> indexDist(0, integerChr.genes.size() - 1);
            size_t idx1 = indexDist(rng);
            size_t idx2 = indexDist(rng);
            if (idx1 > idx2) std::swap(idx1, idx2);
            
            std::reverse(integerChr.genes.begin() + idx1, integerChr.genes.begin() + idx2 + 1);
        }
    }
};

}

auto c_v_r_p_feasibility = [](const Chromosome& chromosome, CVRPEncoder& encoder) {
    const auto& intChr = dynamic_cast<const IntegerChromosome&>(chromosome);
    vector<Route> routes = encoder.decode(intChr);
    
    for (const auto& route : routes) {
        if (encoder.routeLoad(route) > encoder.getInstance().capacity) {
            return false;
        }
    }
    return true;
};
