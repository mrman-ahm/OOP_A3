#include "World.h"
#include "FractalSprawler.h"
#include "KineticHunter.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    srand(time(0));

    World world(25, 60);

    // spawners spread across grid
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

    // hunters at corners
    world.app_Organism(new KineticHunter(0, 0));
    world.app_Organism(new KineticHunter(57, 22));
    world.app_Organism(new KineticHunter(0, 22));
    world.app_Organism(new KineticHunter(57, 0));

    int i = 0;
    while (true)
    {
        cout << "=== Iteration " << ++i << " ===\n";
        world.runIteration();
        system("pause");
    }

    return 0;
}