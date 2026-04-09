#pragma once
#include "Tile.h"
#include "Organism.h"

//Done

class World{
private:
	
	//Mumber variables

	int width, height;  //required to initialize the Tile grid
	
	Tile** grid;
	Organism** population;

	int total_population;   //variables to dnamical manage
	int limit_population;

public:

	//Constructor / Destructor
	World(int height, int width);
	~World();

	//Member Functions
	void runIteration();
	void display();
	void app_Organism(Organism* newOrganism);
};