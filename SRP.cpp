#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <numeric>

using namespace std;

// Each solution contains its total distance and fitness
struct Solution {
    double distance;
    double fitness;
};

// Calculate fitness: smaller distance means better fitness
void calculateFitness(vector<Solution>& population) {
    for (int i = 0; i < population.size(); i++) {
        if (population[i].distance > 0) {
            population[i].fitness = 1.0 / population[i].distance;
        } else {
            population[i].fitness = 0;
        }
    }
}

// Roulette Wheel Selection
int rouletteWheelSelection(const vector<Solution>& population) {
    double totalFitness = 0;

    for (int i = 0; i < population.size(); i++) {
        totalFitness += population[i].fitness;
    }

    if (totalFitness <= 0) {
        return rand() % population.size();
    }

    double randomValue =
        ((double)rand() / RAND_MAX) * totalFitness;

    double cumulativeFitness = 0;

    for (int i = 0; i < population.size(); i++) {
        cumulativeFitness += population[i].fitness;

        if (randomValue <= cumulativeFitness) {
            return i;
        }
    }

    return population.size() - 1;
}

// Tournament Selection
int tournamentSelection(
    const vector<Solution>& population,
    int tournamentSize
) {
    int bestIndex = rand() % population.size();

    for (int i = 1; i < tournamentSize; i++) {
        int candidateIndex = rand() % population.size();

        if (population[candidateIndex].fitness >
            population[bestIndex].fitness) {
            bestIndex = candidateIndex;
        }
    }

    return bestIndex;
}

// Generational Replacement
// The entire new population replaces the old population
vector<Solution> generationalReplacement(
    const vector<Solution>& newPopulation
) {
    return newPopulation;
}

// Elitist Replacement
// Keep the best solutions from both populations
vector<Solution> elitistReplacement(
    const vector<Solution>& oldPopulation,
    const vector<Solution>& newPopulation
) {
    vector<Solution> combined = oldPopulation;

    for (int i = 0; i < newPopulation.size(); i++) {
        combined.push_back(newPopulation[i]);
    }

    sort(combined.begin(), combined.end(),
         [](const Solution& a, const Solution& b) {
             return a.distance < b.distance;
         });

    vector<Solution> result;
    int populationSize = oldPopulation.size();

    for (int i = 0;
         i < populationSize && i < combined.size();
         i++) {
        result.push_back(combined[i]);
    }

    return result;
}

// Display population
void printPopulation(const vector<Solution>& population) {
    for (int i = 0; i < population.size(); i++) {
        cout << "Solution " << i + 1
             << " | Distance: " << population[i].distance
             << " | Fitness: " << population[i].fitness
             << endl;
    }
}

// Find best distance
double findBestDistance(const vector<Solution>& population) {
    double best = population[0].distance;

    for (int i = 1; i < population.size(); i++) {
        if (population[i].distance < best) {
            best = population[i].distance;
        }
    }

    return best;
}

// Find worst distance
double findWorstDistance(const vector<Solution>& population) {
    double worst = population[0].distance;

    for (int i = 1; i < population.size(); i++) {
        if (population[i].distance > worst) {
            worst = population[i].distance;
        }
    }

    return worst;
}

// Find average distance
double findAverageDistance(const vector<Solution>& population) {
    double total = 0;

    for (int i = 0; i < population.size(); i++) {
        total += population[i].distance;
    }

    return total / population.size();
}

// Display performance statistics
void showPerformance(const vector<Solution>& population) {
    cout << fixed << setprecision(2);

    cout << "Best distance: "
         << findBestDistance(population) << endl;

    cout << "Average distance: "
         << findAverageDistance(population) << endl;

    cout << "Worst distance: "
         << findWorstDistance(population) << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    // Example old population
    vector<Solution> oldPopulation = {
        {100, 0},
        {80, 0},
        {120, 0},
        {70, 0}
    };

    calculateFitness(oldPopulation);

    cout << "===== INITIAL POPULATION =====\n";
    printPopulation(oldPopulation);

    // Test Roulette Wheel Selection
    cout << "\n===== ROULETTE WHEEL SELECTION =====\n";

    for (int i = 0; i < 5; i++) {
        int index = rouletteWheelSelection(oldPopulation);

        cout << "Selected distance: "
             << oldPopulation[index].distance << endl;
    }

    // Test Tournament Selection
    cout << "\n===== TOURNAMENT SELECTION =====\n";

    for (int i = 0; i < 5; i++) {
        int index = tournamentSelection(oldPopulation, 2);

        cout << "Selected distance: "
             << oldPopulation[index].distance << endl;
    }

    // Example new population
    vector<Solution> newPopulation = {
        {65, 0},
        {90, 0},
        {75, 0},
        {85, 0}
    };

    calculateFitness(newPopulation);

    // Test Generational Replacement
    vector<Solution> generation1 =
        generationalReplacement(newPopulation);

    cout << "\n===== GENERATIONAL REPLACEMENT =====\n";
    printPopulation(generation1);

    // Test Elitist Replacement
    vector<Solution> generation2 =
        elitistReplacement(oldPopulation, newPopulation);

    cout << "\n===== ELITIST REPLACEMENT =====\n";
    printPopulation(generation2);

    // Performance evaluation
    cout << "\n===== PERFORMANCE RESULTS =====\n";
    showPerformance(generation2);

    return 0;
}