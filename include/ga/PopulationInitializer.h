#pragma once

#include "Chromosome.h"

#include <functional>
#include <memory>
#include <vector>

namespace ga {

/**
 * Creates the initial population.
 *
 * This is a generic hook because different optimization problems can
 * require different chromosome initialization rules.
 */
using PopulationInitializer =
    std::function<std::vector<std::shared_ptr<Chromosome>>(std::size_t)>;

} // namespace ga
