#pragma once
#include "Tile.h"

//Done

class Organism {
protected:       // needed for the derived classes
	int x, y;
	int width, height;
	bool** internalShape; 

public:
	//Constructor / Destructor
	Organism(int x, int y, int width, int height);
	virtual ~Organism();
	
	//Member Functions
	
	virtual void update(Tile** world, int worldWidth, int worldHeight) = 0;
	float getAverageNutrients(Tile** world);

	//Getters
	int getX() { return x; }
	int getY() { return y; }
	int getW() { return width; }
	int getH() { return height; }
	bool isAliveAt(int r, int c) { return internalShape[r][c]; }
	virtual int getType() = 0;
};