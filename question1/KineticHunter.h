#pragma once
#include "Organism.h"

class KineticHunter: public Organism
{
public:
	KineticHunter(int x, int y);
	void update(Tile** world, int worldWidth, int worldHeight) override;
	int getType() override { return 2; }
};