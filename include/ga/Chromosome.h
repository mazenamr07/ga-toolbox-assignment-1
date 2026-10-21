#pragma once

#include <memory>
#include <string>

namespace ga {

/**
 * Base interface for every chromosome representation.
 *
 * Member 2 will implement:
 *   - BinaryChromosome
 *   - IntegerChromosome
 *   - FloatingPointChromosome
 *
 * The GA engine never needs to know how genes are stored internally.
 */
class Chromosome {
public:
    virtual ~Chromosome() = default;

    virtual std::unique_ptr<Chromosome> clone() const = 0;
    virtual std::string representationName() const = 0;
};

} // namespace ga
