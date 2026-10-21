# GA Toolbox — Soft Computing Assignment 1

Cairo University — Faculty of Computing and Artificial Intelligence  
Department of Computer Science  
Soft Computing

This repository implements a Genetic Algorithm toolbox from scratch for Assignment 1.

## Assignment requirements covered by the architecture

The toolbox is designed to support:
- binary, integer, and floating-point chromosomes
- two selection methods
- multi-point crossover and order crossover
- three mutation methods
- two replacement methods
- modular object-oriented reuse
- configurable GA hyperparameters
- user-provided fitness function
- user-provided feasibility function
- generic infeasibility handling

The concrete operators and CVRP implementation are being developed by the other team members.

## Current branch

This branch contains Member 1's foundation:
- GA configuration
- generic chromosome interface
- individual/population model
- fitness and feasibility callbacks
- population initialization callback
- selection/crossover/mutation/replacement interfaces
- main GA engine
- generic infeasibility penalty
- CMake build
- architecture documentation
- engine smoke test

## Build

```bash
cmake -S . -B build
cmake --build build
```

Run the smoke test:

```bash
./build/member1_smoke_test
```

On Windows with a Visual Studio generator, the executable may be under a configuration directory, e.g.:

```text
build/Debug/member1_smoke_test.exe
```

## Important

The default hyperparameter values in `GAConfig` are example starting values. The assignment PDF does not prescribe specific values.

The exact selection, mutation, and replacement methods must match the methods covered in the team's lectures/labs.

## Before merging other members

All members should read:
- `docs/ARCHITECTURE.md`
- `docs/INTERFACES.md`

Do not change the shared interfaces casually after implementation begins. If an interface must change, discuss it with the whole team first.
