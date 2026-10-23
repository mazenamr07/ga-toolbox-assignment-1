#pragma once
#include <vector>
#include <cmath>
#include <cstdio>
#include "Chromosome.h"
using namespace std;

// A route is a list of customer numbers (the depot is not stored).
typedef vector<int> Route;

// ---------------------------------------------------------------
// The CVRP problem data. Node 0 is the depot, nodes 1..n are customers.
// ---------------------------------------------------------------
class CVRPInstance {
public:
    int numCustomers;
    int numVehicles;
    int capacity;
    vector<double> x, y;            // coordinates of every node
    vector<int> demand;             // demand of every node (depot = 0)
    vector<vector<double> > dist;   // dist[i][j] = distance between node i and j

    CVRPInstance(vector<double> x, vector<double> y, vector<int> demand,
                 int capacity, int numVehicles) {
        this->x = x;
        this->y = y;
        this->demand = demand;
        this->capacity = capacity;
        this->numVehicles = numVehicles;
        numCustomers = x.size() - 1;

        // distance between every pair of nodes
        dist.resize(x.size());
        for (int i = 0; i < (int)x.size(); i++) {
            for (int j = 0; j < (int)x.size(); j++) {
                double dx = x[i] - x[j];
                double dy = y[i] - y[j];
                dist[i].push_back(sqrt(dx * dx + dy * dy));
            }
        }
    }
};

// ---------------------------------------------------------------
// CVRP encoding / decoding
//
// Chromosome = permutation of the numbers 1 .. (n + k - 1)
//   n = number of customers, k = number of vehicles
//   1 .. n          -> customers
//   n+1 .. n+k-1    -> separators between routes (k-1 of them)
// The depot (0) is not in the chromosome: every route starts and ends there.
//
// Example: n = 6, k = 3, separators are 7 and 8
//   chromosome [3 1 7 5 2 8 6 4]
//   routes     [3 1] [5 2] [6 4]
//
// Each separator has a different number so the chromosome is still a
// permutation (this is needed for order crossover).
// ---------------------------------------------------------------
class CVRPEncoder {
public:
    CVRPInstance inst;

    CVRPEncoder(CVRPInstance instance) : inst(instance) {}

    int chromosomeLength() const {
        return inst.numCustomers + inst.numVehicles - 1;
    }

    // a random chromosome
    IntegerChromosome randomChromosome() const {
        return IntegerChromosome::randomPermutation(1, chromosomeLength());
    }

    // chromosome -> list of routes (empty routes are skipped)
    vector<Route> decode(const IntegerChromosome &c) const {
        vector<Route> routes;
        Route current;

        for (int i = 0; i < c.size(); i++) {
            int gene = c.genes[i];

            if (gene > inst.numCustomers) {     // it is a separator
                if (!current.empty())
                    routes.push_back(current);
                current.clear();
            } else {                            // it is a customer
                current.push_back(gene);
            }
        }
        if (!current.empty())
            routes.push_back(current);

        return routes;
    }

    // list of routes -> chromosome (used for testing)
    IntegerChromosome encode(vector<Route> routes) const {
        vector<int> genes;
        int separator = inst.numCustomers + 1;

        for (int r = 0; r < inst.numVehicles; r++) {
            if (r < (int)routes.size()) {
                for (int i = 0; i < (int)routes[r].size(); i++)
                    genes.push_back(routes[r][i]);
            }
            if (r < inst.numVehicles - 1) {
                genes.push_back(separator);
                separator++;
            }
        }
        return IntegerChromosome(genes, 1, chromosomeLength(), true);
    }

    // total demand of one route
    int routeLoad(const Route &route) const {
        int load = 0;
        for (int i = 0; i < (int)route.size(); i++)
            load += inst.demand[route[i]];
        return load;
    }

    // distance of one route: depot -> customers -> depot
    double routeDistance(const Route &route) const {
        double total = 0;
        int previous = 0;                       // start at the depot
        for (int i = 0; i < (int)route.size(); i++) {
            total += inst.dist[previous][route[i]];
            previous = route[i];
        }
        total += inst.dist[previous][0];        // go back to the depot
        return total;
    }

    // distance of all routes
    double totalDistance(const vector<Route> &routes) const {
        double total = 0;
        for (int i = 0; i < (int)routes.size(); i++)
            total += routeDistance(routes[i]);
        return total;
    }

    // print in the assignment output format
    void printSolution(const vector<Route> &routes) const {
        for (int i = 0; i < (int)routes.size(); i++) {
            printf("Route #%d:", i + 1);
            for (int j = 0; j < (int)routes[i].size(); j++)
                printf(" %d", routes[i][j]);
            printf("\n");
        }
        printf("Cost %.2f\n", totalDistance(routes));
    }
};
