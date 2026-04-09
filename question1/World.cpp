#include "World.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//Done

World::World(int height, int width)
	: height(height), width(width), total_population(0), limit_population(10)
{
	population = new Organism *[limit_population];
	
	grid = new Tile * [height];
	for (int i = 0;i < height; i++)
	{
		grid[i] = new Tile[width];

		for (int j = 0; j < width; j++)
		{
            grid[i][j].nutrientLevel = (float)(rand() % 101);  
            grid[i][j].toxicity = 0;
            grid[i][j].occupant = nullptr;
		}
	}
}

World::~World()
{
	//population cleanup
	for (int i = 0; i < total_population;i++)
		delete population[i];
	delete[] population;

	// grid cleanup
	for (int i = 0; i < height;i++)
		delete[] grid[i];
	delete[] grid;
}

void World::app_Organism(Organism* newOrganism)
{
	//resizing 
	if (total_population >= limit_population)
	{
		limit_population *= 2;

		Organism** temp = new Organism * [limit_population];

		for (int i = 0; i < total_population;i++)
			temp[i] = population[i];
		delete[] population;

		population = temp;
	}

	//appending next one
	population[total_population++] = newOrganism;
}

void World::runIteration()
{
    // clear occupants
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            grid[i][j].occupant = nullptr;

    // stamp before update
    for (int i = 0; i < total_population; i++)
    {
        Organism* org = population[i];
        for (int r = 0; r < org->getH(); r++)
            for (int c = 0; c < org->getW(); c++)
                if (org->isAliveAt(r, c))
                {
                    int wy = org->getY() + r;
                    int wx = org->getX() + c;
                    if (wy >= 0 && wy < height && wx >= 0 && wx < width)
                        grid[wy][wx].occupant = org;
                }
    }

    // updates
    for (int i = 0; i < total_population; i++)
        population[i]->update(grid, width, height);

    // remove dead and clear from grid
    int alive = 0;
    for (int i = 0; i < total_population; i++)
    {
        if (population[i]->getW() == 0 || population[i]->getH() == 0)
        {
            for (int r = 0; r < height; r++)
                for (int c = 0; c < width; c++)
                    if (grid[r][c].occupant == population[i])
                        grid[r][c].occupant = nullptr;
            delete population[i];
        }
        else
            population[alive++] = population[i];
    }
    total_population = alive;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j].occupant == nullptr) continue;
            bool found = false;
            for (int k = 0; k < total_population; k++)
                if (population[k] == grid[i][j].occupant)
                {
                    found = true; break;
                }
            if (!found)
            {
                app_Organism(grid[i][j].occupant);
                grid[i][j].occupant = nullptr;  // ADD THIS
            }
        }

    // re-stamp for display
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            grid[i][j].occupant = nullptr;

    for (int i = 0; i < total_population; i++)
    {
        Organism* org = population[i];
        for (int r = 0; r < org->getH(); r++)
            for (int c = 0; c < org->getW(); c++)
                if (org->isAliveAt(r, c))
                {
                    int wy = org->getY() + r;
                    int wx = org->getX() + c;
                    if (wy >= 0 && wy < height && wx >= 0 && wx < width)
                        grid[wy][wx].occupant = org;
                }
    }

    display();
}

void World::display()
{
    float totalToxicity = 0.0f;
    // Using unsigned char to ensure 219 is treated as the correct extended ASCII block
    unsigned char block = 219;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            totalToxicity += grid[i][j].toxicity;

            if (grid[i][j].occupant == nullptr) {
                // "Use Black Squares for empty tiles"
                // \033[30m is the ANSI code for BLACK
                // We print two blocks to make it look like a square
                cout << "\033[30m" << block << block << "\033[0m";
            }
            else if (grid[i][j].occupant->getType() == 1) {
                // "Use Green Squares for Producer (Fractal Sprawler) tiles"
                // \033[32m is Green
                cout << "\033[32m" << block << block << "\033[0m";
            }
            else {
                // "Use Red Squares for Consumer (Kinetic Hunter) tiles"
                // \033[31m is Red
                cout << "\033[31m" << block << block << "\033[0m";
            }
        }
        cout << "\n";
    }

    // Display required stats after every iteration
    cout << "Total Population: " << total_population << "\n";
    // Ensure float division for the average
    cout << "Average World Toxicity: " << totalToxicity / (float)(width * height) << "\n\n";
}