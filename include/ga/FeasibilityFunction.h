#pragma once

#include "Chromosome.h"

#include <functional>

namespace ga {

/**
 * Returns true when a candidate satisfies the optimization problem's
 * constraints.
 *
 * The generic GA engine does not contain CVRP-specific constraints.
 */
using FeasibilityFunction = std::function<bool(const Chromosome&)>;

} // namespace ga
