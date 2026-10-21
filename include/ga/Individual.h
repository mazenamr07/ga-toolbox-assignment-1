#pragma once

#include "Chromosome.h"

#include <memory>
#include <limits>

namespace ga {

/**
 * One candidate solution in the population.
 *
 * rawFitness:
 *   Value returned by the problem's fitness/objective function.
 *
 * fitness:
 *   Value after generic infeasibility handling (penalty, if enabled).
 */
struct Individual {
    std::shared_ptr<Chromosome> chromosome;

    double rawFitness = std::numeric_limits<double>::quiet_NaN();
    double fitness = std::numeric_limits<double>::quiet_NaN();

    bool feasible = false;
    bool evaluated = false;
};

} // namespace ga
