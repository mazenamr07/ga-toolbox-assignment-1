#pragma once

#include "Population.h"

#include <memory>
#include <random>

namespace ga {

/**
 * Replacement operator interface.
 *
 * Member 5 will implement the two replacement methods required by the
 * course.
 */
class Replacement {
public:
    virtual ~Replacement() = default;

    virtual Population replace(
        const Population& current,
        Population offspring,
        std::mt19937& rng) = 0;
};

using ReplacementPtr = std::shared_ptr<Replacement>;

} // namespace ga
