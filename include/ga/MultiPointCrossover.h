#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include "ga/Crossover.h"
#include "Chromosome.h"

namespace ga {

class MultiPointCrossover : public Crossover {
private:
    int numPoints;

    template <class C>
    static std::vector<std::shared_ptr<Chromosome>> apply(
        const C& p1, const C& p2, int k, std::mt19937& rng)
    {
        int n = p1.size();
        if (n != p2.size())
            throw std::invalid_argument("Parents must have the same length.");

        C c1 = p1;
        C c2 = p2;

        if (n >= 2) {
            k = std::min(k, n - 1);
            std::vector<int> pts;
            std::uniform_int_distribution<int> dist(1, n - 1);
            while ((int)pts.size() < k) {
                int p = dist(rng);
                if (std::find(pts.begin(), pts.end(), p) == pts.end())
                    pts.push_back(p);
            }
            std::sort(pts.begin(), pts.end());

            bool swapNow = false;
            int pi = 0;
            for (int i = 0; i < n; i++) {
                if (pi < (int)pts.size() && i == pts[pi]) {
                    swapNow = !swapNow;
                    pi++;
                }
                if (swapNow) {
                    c1.genes[i] = p2.genes[i];
                    c2.genes[i] = p1.genes[i];
                }
            }
        }
        return { std::make_shared<C>(c1), std::make_shared<C>(c2) };
    }

public:
    explicit MultiPointCrossover(int points = 2) : numPoints(points) {
        if (points < 1)
            throw std::invalid_argument("Number of crossover points must be >= 1.");
    }

    std::vector<std::shared_ptr<Chromosome>> crossover(
        const Chromosome& parent1,
        const Chromosome& parent2,
        std::mt19937& rng) override
    {
        if (auto a = dynamic_cast<const IntegerChromosome*>(&parent1)) {
            const auto& b = dynamic_cast<const IntegerChromosome&>(parent2);
            if (a->isPermutation)
                throw std::invalid_argument(
                    "Multi-point crossover would duplicate genes in a permutation; use Order1Crossover.");
            return apply(*a, b, numPoints, rng);
        }
        if (auto a = dynamic_cast<const BinaryChromosome*>(&parent1))
            return apply(*a, dynamic_cast<const BinaryChromosome&>(parent2), numPoints, rng);

        if (auto a = dynamic_cast<const FloatChromosome*>(&parent1))
            return apply(*a, dynamic_cast<const FloatChromosome&>(parent2), numPoints, rng);

        throw std::invalid_argument("MultiPointCrossover: unsupported chromosome type.");
    }
};

}
