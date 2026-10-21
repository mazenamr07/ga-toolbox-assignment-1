#pragma once

#include "Chromosome.h"

#include <memory>
#include <random>
#include <vector>

namespace ga {

/**
 * Crossover operator interface.
 *
 * Member 3 will implement:
 *   - Multi-point crossover
 *   - Order crossover
 */
class Crossover {
public:
    virtual ~Crossover() = default;

    virtual std::vector<std::shared_ptr<Chromosome>> crossover(
        const Chromosome& parent1,
        const Chromosome& parent2,
        std::mt19937& rng) = 0;
};

using CrossoverPtr = std::shared_ptr<Crossover>;

} // namespace ga
