#pragma once

#include "Crossover.h"
#include "FeasibilityFunction.h"
#include "FitnessFunction.h"
#include "GAConfig.h"
#include "Mutation.h"
#include "Population.h"
#include "PopulationInitializer.h"
#include "Replacement.h"
#include "Selection.h"

#include <cstddef>
#include <memory>
#include <random>
#include <stdexcept>

namespace ga {

/**
 * Main reusable Genetic Algorithm engine.
 *
 * Responsibilities:
 *   - validate configuration and required dependencies
 *   - create the initial population
 *   - evaluate candidates using user-provided callbacks
 *   - select parents using the selected operator
 *   - apply crossover and mutation
 *   - evaluate offspring
 *   - apply replacement
 *   - track and return the best solution
 *
 * It deliberately does NOT contain:
 *   - binary/integer/floating chromosome internals
 *   - a specific selection algorithm
 *   - a specific crossover algorithm
 *   - a specific mutation algorithm
 *   - a specific replacement algorithm
 *   - CVRP-specific fitness or constraints
 */
class GAEngine {
public:
    explicit GAEngine(
        GAConfig config,
        PopulationInitializer initializer,
        FitnessFunction fitnessFunction,
        FeasibilityFunction feasibilityFunction,
        SelectionPtr selection,
        CrossoverPtr crossover,
        MutationPtr mutation,
        ReplacementPtr replacement,
        std::uint32_t seed = std::random_device{}());

    /**
     * Runs the GA and returns the best individual found.
     *
     * The current implementation creates two offspring per mating event.
     * If the configured population size is odd, the last offspring is
     * truncated by the replacement strategy or by the engine as needed.
     */
    Individual run();

    const GAConfig& config() const noexcept { return config_; }

private:
    GAConfig config_;

    PopulationInitializer initializer_;
    FitnessFunction fitnessFunction_;
    FeasibilityFunction feasibilityFunction_;

    SelectionPtr selection_;
    CrossoverPtr crossover_;
    MutationPtr mutation_;
    ReplacementPtr replacement_;

    std::mt19937 rng_;

    void evaluate(Individual& individual) const;
    void evaluatePopulation(Population& population) const;

    static bool better(const Individual& lhs,
                       const Individual& rhs,
                       OptimizationGoal goal);

    static const Individual& bestOf(const Population& population,
                                    OptimizationGoal goal);
};

} // namespace ga
