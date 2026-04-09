#pragma once
#include "Organism.h"

class FractalSprawler : public Organism
{
private:
	void grow();
public:
	void shrink();
	FractalSprawler(int x, int  y);
	void update(Tile** world, int worldWidth, int worldHeight) override;
	int getType() override { return 1; }
};