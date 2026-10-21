#include "ga/GAEngine.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace ga {

GAEngine::GAEngine(
    GAConfig config,
    PopulationInitializer initializer,
    FitnessFunction fitnessFunction,
    FeasibilityFunction feasibilityFunction,
    SelectionPtr selection,
    CrossoverPtr crossover,
    MutationPtr mutation,
    ReplacementPtr replacement,
    std::uint32_t seed)
    : config_(std::move(config)),
      initializer_(std::move(initializer)),
      fitnessFunction_(std::move(fitnessFunction)),
      feasibilityFunction_(std::move(feasibilityFunction)),
      selection_(std::move(selection)),
      crossover_(std::move(crossover)),
      mutation_(std::move(mutation)),
      replacement_(std::move(replacement)),
      rng_(seed) {
    config_.validate();

    if (!initializer_) {
        throw std::invalid_argument("A population initializer is required.");
    }
    if (!fitnessFunction_) {
        throw std::invalid_argument("A fitness function is required.");
    }
    if (!feasibilityFunction_) {
        throw std::invalid_argument("A feasibility function is required.");
    }
    if (!selection_) {
        throw std::invalid_argument("A selection operator is required.");
    }
    if (!crossover_) {
        throw std::invalid_argument("A crossover operator is required.");
    }
    if (!mutation_) {
        throw std::invalid_argument("A mutation operator is required.");
    }
    if (!replacement_) {
        throw std::invalid_argument("A replacement operator is required.");
    }
}

void GAEngine::evaluate(Individual& individual) const {
    if (!individual.chromosome) {
        throw std::invalid_argument("Individual contains a null chromosome.");
    }

    individual.rawFitness = fitnessFunction_(*individual.chromosome);
    individual.feasible = feasibilityFunction_(*individual.chromosome);

    if (!std::isfinite(individual.rawFitness)) {
        throw std::invalid_argument("Fitness function returned a non-finite value.");
    }

    individual.fitness = individual.rawFitness;

    // Generic infeasibility handling chosen for this toolbox:
    // penalty. The sign depends on whether the problem is minimized
    // or maximized.
    if (!individual.feasible &&
        config_.infeasibleHandling == InfeasibleHandling::Penalty) {
        if (config_.goal == OptimizationGoal::Minimize) {
            individual.fitness += config_.infeasiblePenalty;
        } else {
            individual.fitness -= config_.infeasiblePenalty;
        }
    }

    individual.evaluated = true;
}

void GAEngine::evaluatePopulation(Population& population) const {
    for (auto& individual : population) {
        evaluate(individual);
    }
}

bool GAEngine::better(
    const Individual& lhs,
    const Individual& rhs,
    OptimizationGoal goal) {
    if (goal == OptimizationGoal::Minimize) {
        return lhs.fitness < rhs.fitness;
    }
    return lhs.fitness > rhs.fitness;
}

const Individual& GAEngine::bestOf(
    const Population& population,
    OptimizationGoal goal) {
    if (population.empty()) {
        throw std::invalid_argument("Cannot find best individual in an empty population.");
    }

    return *std::min_element(
        population.begin(),
        population.end(),
        [goal](const Individual& a, const Individual& b) {
            return better(a, b, goal);
        });
}

Individual GAEngine::run() {
    auto chromosomes = initializer_(config_.populationSize);

    if (chromosomes.size() != config_.populationSize) {
        throw std::invalid_argument(
            "Population initializer returned an unexpected population size.");
    }

    Population population;
    population.reserve(chromosomes.size());

    for (auto& chromosome : chromosomes) {
        if (!chromosome) {
            throw std::invalid_argument(
                "Population initializer returned a null chromosome.");
        }

        Individual individual;
        individual.chromosome = std::move(chromosome);
        population.push_back(std::move(individual));
    }

    evaluatePopulation(population);

    Individual best = bestOf(population, config_.goal);
    std::size_t generationsWithoutImprovement = 0;

    std::uniform_real_distribution<double> probability(0.0, 1.0);

    for (std::size_t generation = 0;
         generation < config_.maxGenerations;
         ++generation) {

        Population offspring;
        offspring.reserve(config_.populationSize);

        while (offspring.size() < config_.populationSize) {
            const std::size_t parent1Index = selection_->select(population);
            const std::size_t parent2Index = selection_->select(population);

            if (parent1Index >= population.size() ||
                parent2Index >= population.size()) {
                throw std::out_of_range(
                    "Selection operator returned an invalid population index.");
            }

            const auto& parent1 = population[parent1Index];
            const auto& parent2 = population[parent2Index];

            std::vector<std::shared_ptr<Chromosome>> children;

            if (probability(rng_) <= config_.crossoverRate) {
                children = crossover_->crossover(
                    *parent1.chromosome,
                    *parent2.chromosome,
                    rng_);
            } else {
                children.push_back(
                    std::shared_ptr<Chromosome>(parent1.chromosome->clone().release()));
                children.push_back(
                    std::shared_ptr<Chromosome>(parent2.chromosome->clone().release()));
            }

            for (auto& childChromosome : children) {
                if (!childChromosome) {
                    throw std::invalid_argument(
                        "Crossover returned a null child chromosome.");
                }

                if (probability(rng_) <= config_.mutationRate) {
                    mutation_->mutate(*childChromosome, rng_);
                }

                Individual child;
                child.chromosome = std::move(childChromosome);
                offspring.push_back(std::move(child));

                if (offspring.size() == config_.populationSize) {
                    break;
                }
            }

            // Protect against a broken/custom crossover implementation
            // that returns no children and would otherwise cause an infinite loop.
            if (children.empty()) {
                throw std::runtime_error(
                    "Crossover produced no offspring.");
            }
        }

        evaluatePopulation(offspring);

        population = replacement_->replace(
            population,
            std::move(offspring),
            rng_);

        if (population.empty()) {
            throw std::runtime_error(
                "Replacement produced an empty population.");
        }

        if (population.size() != config_.populationSize) {
            throw std::runtime_error(
                "Replacement returned an unexpected population size.");
        }

        evaluatePopulation(population);

        const Individual& generationBest = bestOf(population, config_.goal);

        if (better(generationBest, best, config_.goal)) {
            best = generationBest;
            generationsWithoutImprovement = 0;
        } else {
            ++generationsWithoutImprovement;
        }

        if (config_.stagnationLimit > 0 &&
            generationsWithoutImprovement >= config_.stagnationLimit) {
            break;
        }
    }

    return best;
}

} // namespace ga
