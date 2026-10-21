# GA Toolbox Architecture — Member 1

## Purpose

The toolbox is a reusable, object-oriented Genetic Algorithm framework.
The GA engine is intentionally independent of CVRP-specific rules.

## Responsibility split

### Member 1
- Repository/build structure
- GA configuration
- Fitness callback interface
- Feasibility callback interface
- Population initialization interface
- GA engine/control loop
- Integration interfaces
- Generic infeasibility handling using a penalty

### Member 2
- Binary chromosome
- Integer chromosome
- Floating-point chromosome
- CVRP representation and decoding

### Member 3
- Multi-point crossover
- Order crossover
- CVRP fitness function

### Member 4
- Three mutation methods
- CVRP feasibility checking
- Any CVRP-specific repair logic if the team chooses repair instead of, or in addition to, the generic penalty

### Member 5
- Two selection methods
- Two replacement methods

## Main flow

```text
PopulationInitializer
        |
        v
  Initial Population
        |
        v
   Fitness + Feasibility
        |
        v
      Selection
        |
        v
      Crossover
        |
        v
       Mutation
        |
        v
   Fitness + Feasibility
        |
        v
     Replacement
        |
        +------> next generation
```

## Important boundary

The generic engine must NOT contain CVRP-specific rules.

For CVRP:
- The fitness callback calculates route cost.
- The feasibility callback checks route constraints.
- The chromosome/decoder defines how the solution is represented.

The engine only knows that it can call these functions.

## Fitness convention

`GAConfig::goal` determines whether lower or higher values are better.

For CVRP, use:

```cpp
config.goal = ga::OptimizationGoal::Minimize;
```

and return total routing cost from the fitness callback.

## Infeasible solutions

The current generic strategy is a penalty.

For minimization:
`adjusted fitness = raw fitness + penalty`

For maximization:
`adjusted fitness = raw fitness - penalty`

The raw fitness is retained separately in `Individual::rawFitness`.

The penalty is deliberately generic. It does not know why a solution is infeasible.

## Operator contracts

### Selection

Returns an index into the current population.

### Crossover

Receives two parent chromosomes and returns one or more child chromosomes.

### Mutation

Modifies one chromosome in place.

### Replacement

Receives the current population and offspring and returns the next population.

### Fitness

Receives a chromosome and returns a numeric objective value.

### Feasibility

Receives a chromosome and returns `true` if constraints are satisfied.

## Ownership

Chromosomes are held by `std::shared_ptr` inside `Individual`.

Crossover creates new chromosome objects.

The engine clones parents when crossover is skipped.

This avoids sharing the same mutable chromosome object between parents and offspring.
