#pragma once
#include <vector>
#include <stdexcept>
#include "ga/FitnessFunction.h"
#include "CVRP.h"
#include "Chromosome.h"

class CVRPFitnessFunction {
private:
    CVRPEncoder encoder;

public:
    explicit CVRPFitnessFunction(const CVRPInstance& instance) : encoder(instance) {}

    double operator()(const ga::Chromosome& chromosome) const {
        const auto* intChrom = dynamic_cast<const IntegerChromosome*>(&chromosome);
        if (!intChrom)
            throw std::invalid_argument("CVRPFitnessFunction expects an IntegerChromosome.");

        std::vector<Route> routes = encoder.decode(*intChrom);
        return encoder.totalDistance(routes);
    }
};
