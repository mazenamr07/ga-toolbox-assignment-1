#pragma once

#include "Chromosome.h"

#include <functional>

namespace ga {

/**
 * Returns the raw objective/fitness value for one chromosome.
 *
 * The meaning of "better" is controlled by GAConfig::goal.
 * For example, CVRP normally uses total route distance, so the
 * toolbox can be configured for minimization.
 */
using FitnessFunction = std::function<double(const Chromosome&)>;

} // namespace ga
