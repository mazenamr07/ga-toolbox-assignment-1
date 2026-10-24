#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include "ga/Crossover.h"
#include "Chromosome.h"

namespace ga {

class Order1Crossover : public Crossover {
private:
    static IntegerChromosome buildChild(
        const IntegerChromosome& a, const IntegerChromosome& b, int cut1, int cut2)
    {
        int n = a.size();
        std::vector<int> g(n, -1);
        std::vector<bool> used(a.high + 1, false);

        for (int i = cut1; i <= cut2; i++) {
            g[i] = a.genes[i];
            used[g[i]] = true;
        }

        int pos = (cut2 + 1) % n;
        for (int i = 0; i < n; i++) {
            int gene = b.genes[(cut2 + 1 + i) % n];
            if (!used[gene]) {
                g[pos] = gene;
                pos = (pos + 1) % n;
            }
        }
        return IntegerChromosome(g, a.low, a.high, true);
    }

public:
    std::vector<std::shared_ptr<Chromosome>> crossover(
        const Chromosome& parent1,
        const Chromosome& parent2,
        std::mt19937& rng) override
    {
        const auto& p1 = dynamic_cast<const IntegerChromosome&>(parent1);
        const auto& p2 = dynamic_cast<const IntegerChromosome&>(parent2);

        if (!p1.isPermutation || !p2.isPermutation)
            throw std::invalid_argument("Order crossover requires permutation chromosomes.");
        if (p1.size() != p2.size())
            throw std::invalid_argument("Parents must have the same length.");

        int n = p1.size();
        if (n < 2) {
            return { std::make_shared<IntegerChromosome>(p1),
                     std::make_shared<IntegerChromosome>(p2) };
        }

        std::uniform_int_distribution<int> dist(0, n - 1);
        int cut1 = dist(rng);
        int cut2 = dist(rng);
        if (cut1 > cut2) std::swap(cut1, cut2);

        return {
            std::make_shared<IntegerChromosome>(buildChild(p1, p2, cut1, cut2)),
            std::make_shared<IntegerChromosome>(buildChild(p2, p1, cut1, cut2))
        };
    }
};

}
