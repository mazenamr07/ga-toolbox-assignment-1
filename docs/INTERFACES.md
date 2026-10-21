# Team Interface Contract

This document should be agreed upon before merging the other members' branches.

## 1. Chromosome

Every chromosome must derive from:

```cpp
class Chromosome {
public:
    virtual ~Chromosome() = default;
    virtual std::unique_ptr<Chromosome> clone() const = 0;
    virtual std::string representationName() const = 0;
};
```

Member 2's three representations must implement this interface.

## 2. Individual

An individual contains:
- chromosome
- raw fitness
- adjusted fitness
- feasibility flag
- evaluated flag

Do not store CVRP-specific fields in `Individual`.

## 3. Selection

```cpp
std::size_t select(const Population& population);
```

The returned value must be a valid index into `population`.

## 4. Crossover

```cpp
std::vector<std::shared_ptr<Chromosome>> crossover(
    const Chromosome& parent1,
    const Chromosome& parent2,
    std::mt19937& rng);
```

The implementation must return at least one child.

If an operator is intended for only one representation, it should validate the representation and fail clearly rather than silently producing invalid chromosomes.

## 5. Mutation

```cpp
void mutate(Chromosome& chromosome, std::mt19937& rng);
```

Mutation must preserve the invariants required by the representation/operator.

For example, a permutation mutation must not accidentally create duplicate customer IDs.

## 6. Replacement

```cpp
Population replace(
    const Population& current,
    Population offspring,
    std::mt19937& rng);
```

The final population must have exactly `GAConfig::populationSize` individuals.

## 7. Fitness

```cpp
double fitness(const Chromosome&);
```

Return the raw problem objective.

Do not apply the generic infeasibility penalty inside the CVRP fitness function; the engine does that.

## 8. Feasibility

```cpp
bool feasible(const Chromosome&);
```

Return only whether the solution satisfies the problem constraints.

Do not modify the chromosome inside this function.

## 9. Randomness

Operators receive the engine's `std::mt19937& rng`.

Do not create a new random engine on every operator call. This keeps runs reproducible when a fixed seed is supplied.

## 10. Build rule

All new source files should be added to `CMakeLists.txt` and compile from a clean build.
