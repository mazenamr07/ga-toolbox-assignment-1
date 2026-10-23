#include <iostream>
#include "CVRP.h"
using namespace std;

void printVector(vector<int> v) {
    for (int i = 0; i < (int)v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

// returns true if every customer 1..n appears exactly once in the routes
bool allCustomersOnce(vector<Route> routes, int n) {
    vector<int> count(n + 1, 0);
    for (int i = 0; i < (int)routes.size(); i++)
        for (int j = 0; j < (int)routes[i].size(); j++)
            count[routes[i][j]]++;
    for (int c = 1; c <= n; c++)
        if (count[c] != 1)
            return false;
    return true;
}

int main() {
    srand(1);

    // ----- test the three chromosome types -----
    BinaryChromosome b(8);
    cout << "Binary:  "; printVector(b.genes);

    IntegerChromosome p = IntegerChromosome::randomPermutation(1, 6);
    cout << "Integer: "; printVector(p.genes);

    FloatChromosome f(4, 0.0, 1.0);
    cout << "Float:   ";
    for (int i = 0; i < f.size(); i++) cout << f.genes[i] << " ";
    cout << endl;

    // ----- small CVRP: depot + 6 customers, 3 vehicles, capacity 10 -----
    double xs[] = {0, 2, 5, 6, -3, -4, 1};
    double ys[] = {0, 3, 1, 6, 4, -2, -5};
    int ds[] = {0, 4, 3, 5, 6, 2, 4};
    vector<double> x(xs, xs + 7), y(ys, ys + 7);
    vector<int> demand(ds, ds + 7);

    CVRPInstance inst(x, y, demand, 10, 3);
    CVRPEncoder encoder(inst);

    // sample chromosome (separators are 7 and 8)
    int g[] = {3, 1, 7, 5, 2, 8, 6, 4};
    IntegerChromosome c(vector<int>(g, g + 8), 1, 8, true);

    vector<Route> routes = encoder.decode(c);
    cout << "\nChromosome: "; printVector(c.genes);
    for (int i = 0; i < (int)routes.size(); i++) {
        cout << "Route " << i + 1 << ": ";
        printVector(routes[i]);
        cout << "  load = " << encoder.routeLoad(routes[i]) << endl;
    }

    // ----- checks -----
    bool ok = true;
    if (routes.size() != 3) ok = false;
    if (encoder.routeLoad(routes[0]) != 9) ok = false;

    // everything in one route: overloaded (24 > 10)
    int g2[] = {1, 2, 3, 4, 5, 6, 7, 8};
    vector<Route> one = encoder.decode(IntegerChromosome(vector<int>(g2, g2 + 8), 1, 8, true));
    if (one.size() != 1 || encoder.routeLoad(one[0]) != 24) ok = false;

    // encode then decode must give the same routes
    IntegerChromosome again = encoder.encode(routes);
    if (encoder.decode(again) != routes) ok = false;

    // random chromosomes must contain every customer exactly once
    for (int t = 0; t < 200; t++) {
        IntegerChromosome r = encoder.randomChromosome();
        if (!allCustomersOnce(encoder.decode(r), 6)) ok = false;
    }

    cout << "\nSolution in output format:" << endl;
    encoder.printSolution(routes);

    if (ok) cout << "\nAll tests passed" << endl;
    else cout << "\nSOME TESTS FAILED" << endl;
    return 0;
}
