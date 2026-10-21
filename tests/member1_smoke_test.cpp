#include "ga/GAEngine.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

using namespace ga;

// Minimal chromosome used only to test Member 1's engine.
// Member 2 will replace this with the real three representations.
class TestChromosome final : public Chromosome {
public:
    explicit TestChromosome(int value) : value(value) {}

    std::unique_ptr<Chromosome> clone() const override {
        return std::make_unique<TestChromosome>(*this);
    }

    std::string representationName() const override {
        return "test";
    }

    int value;
};

class TestSelection final : public Selection {
public:
    std::size_t select(const Population& population) override {
        assert(!population.empty());
        return 0;
    }
};

class TestCrossover final : public Crossover {
public:
    std::vector<std::shared_ptr<Chromosome>> crossover(
        const Chromosome& parent1,
        const Chromosome& parent2,
        std::mt19937&) override {

        const auto& p1 = dynamic_cast<const TestChromosome&>(parent1);
        const auto& p2 = dynamic_cast<const TestChromosome&>(parent2);

        return {
            std::make_shared<TestChromosome>(p1.value),
            std::make_shared<TestChromosome>(p2.value)
        };
    }
};

class TestMutation final : public Mutation {
public:
    void mutate(Chromosome&, std::mt19937&) override {
        // Deliberately does nothing. This test is for the engine.
    }
};

class TestReplacement final : public Replacement {
public:
    Population replace(
        const Population&,
        Population offspring,
        std::mt19937&) override {
        return offspring;
    }
};

int main() {
    GAConfig config;
    config.populationSize = 10;
    config.maxGenerations = 5;
    config.crossoverRate = 1.0;
    config.mutationRate = 0.0;
    config.goal = OptimizationGoal::Minimize;

    auto initializer = [](std::size_t count) {
        std::vector<std::shared_ptr<Chromosome>> result;
        result.reserve(count);

        for (std::size_t i = 0; i < count; ++i) {
            result.push_back(std::make_shared<TestChromosome>(
                static_cast<int>(count - i)));
        }

        return result;
    };

    auto fitness = [](const Chromosome& chromosome) {
        const auto& c = dynamic_cast<const TestChromosome&>(chromosome);
        return static_cast<double>(c.value);
    };

    auto feasible = [](const Chromosome& chromosome) {
        const auto& c = dynamic_cast<const TestChromosome&>(chromosome);
        return c.value >= 0;
    };

    GAEngine engine(
        config,
        initializer,
        fitness,
        feasible,
        std::make_shared<TestSelection>(),
        std::make_shared<TestCrossover>(),
        std::make_shared<TestMutation>(),
        std::make_shared<TestReplacement>(),
        42u
    );

    const Individual best = engine.run();

    assert(best.evaluated);
    assert(best.feasible);
    assert(std::isfinite(best.fitness));
    assert(best.fitness == 1.0);

    std::cout << "Member 1 engine smoke test passed.\n";
    std::cout << "Best fitness: " << best.fitness << '\n';

    return 0;
}
