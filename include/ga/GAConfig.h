#pragma once

#include <cstddef>
#include <stdexcept>

namespace ga {

enum class OptimizationGoal {
    Minimize,
    Maximize
};

enum class InfeasibleHandling {
    Penalty
};

/**
 * User-configurable GA hyperparameters.
 *
 * The default values are intentionally generic starting values.
 * They are not values specified by the assignment.
 */
struct GAConfig {
    std::size_t populationSize = 100;
    std::size_t maxGenerations = 500;

    double crossoverRate = 0.80;
    double mutationRate = 0.05;

    OptimizationGoal goal = OptimizationGoal::Minimize;

    InfeasibleHandling infeasibleHandling = InfeasibleHandling::Penalty;
    double infeasiblePenalty = 1'000'000.0;

    // Stops after this many generations without improvement.
    // 0 disables this criterion.
    std::size_t stagnationLimit = 0;

    void validate() const {
        if (populationSize == 0) {
            throw std::invalid_argument("populationSize must be greater than zero.");
        }

        if (maxGenerations == 0) {
            throw std::invalid_argument("maxGenerations must be greater than zero.");
        }

        if (crossoverRate < 0.0 || crossoverRate > 1.0) {
            throw std::invalid_argument("crossoverRate must be in [0, 1].");
        }

        if (mutationRate < 0.0 || mutationRate > 1.0) {
            throw std::invalid_argument("mutationRate must be in [0, 1].");
        }

        if (infeasiblePenalty < 0.0) {
            throw std::invalid_argument("infeasiblePenalty must not be negative.");
        }
    }
};

} // namespace ga
