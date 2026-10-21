#pragma once

#include "Population.h"

#include <cstddef>
#include <memory>

namespace ga {

/**
 * Selection operator interface.
 *
 * Member 5 will implement the two selection methods required by the
 * course. The engine only depends on this interface.
 */
class Selection {
public:
    virtual ~Selection() = default;

    // Returns an index into the supplied population.
    virtual std::size_t select(const Population& population) = 0;
};

using SelectionPtr = std::shared_ptr<Selection>;

} // namespace ga
