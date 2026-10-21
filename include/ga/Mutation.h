#pragma once

#include "Chromosome.h"

#include <memory>
#include <random>

namespace ga {

/**
 * Mutation operator interface.
 *
 * Member 4 will implement the three mutation methods required by the
 * course.
 */
class Mutation {
public:
    virtual ~Mutation() = default;

    virtual void mutate(Chromosome& chromosome, std::mt19937& rng) = 0;
};

using MutationPtr = std::shared_ptr<Mutation>;

} // namespace ga
