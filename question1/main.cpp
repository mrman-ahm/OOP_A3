#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Header.h"
// ==========================================
// MAIN FUNCTION (Q1.cpp)
// ==========================================
int main() {
    srand(time(0));

    World world(25, 60);

    // Fractal Sprawlers (Producers)
    world.app_Organism(new FractalSprawler(5, 2));
    world.app_Organism(new FractalSprawler(20, 2));
    world.app_Organism(new FractalSprawler(35, 2));
    world.app_Organism(new FractalSprawler(50, 2));
    world.app_Organism(new FractalSprawler(5, 12));
    world.app_Organism(new FractalSprawler(20, 12));
    world.app_Organism(new FractalSprawler(35, 12));
    world.app_Organism(new FractalSprawler(50, 12));
    world.app_Organism(new FractalSprawler(12, 22));
    world.app_Organism(new FractalSprawler(42, 22));

    // Kinetic Hunters (Consumers)
    world.app_Organism(new KineticHunter(0, 0));
    world.app_Organism(new KineticHunter(57, 22));
    world.app_Organism(new KineticHunter(0, 22));
    world.app_Organism(new KineticHunter(57, 0));

    int iteration = 0;
    while (iteration < 20) {
        system("cls");
        cout << "=== Iteration " << ++iteration << " ===\n";
        world.runIteration();

        cout << "Press Enter for next iteration..." << endl;
        cin.get();
    }

    return 0;
}