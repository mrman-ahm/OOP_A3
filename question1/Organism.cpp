#include "Organism.h"

Organism::Organism(int x, int y, int width, int height)
	: x(x), y(y), width(width), height(height)
{
	internalShape = new bool* [height];
	for (int i = 0; i < height;i++)
	{
		internalShape[i] = new bool[width];

		for (int j = 0; j < width; j++)
			internalShape[i][j] = true;
	}
}

Organism::~Organism()
{
	if (internalShape)
	{
		for (int i = 0; i < height; i++)
			delete[] internalShape[i];
		delete[] internalShape;
		internalShape = nullptr;
	}
}

float Organism::getAverageNutrients(Tile** world)
{
    float nutrientSum = 0.0f;
    int no_activeTile = 0;

    for (int ver_body = 0; ver_body < height; ver_body++)
    {
        for (int hor_body = 0; hor_body < width; hor_body++)
        {
            if (internalShape[ver_body][hor_body])
            {
                int wy = y + ver_body;
                int wx = x + hor_body;
                if (wy >= 0 && wx >= 0)   // ADD THIS BOUNDS CHECK
                {
                    nutrientSum += world[wy][wx].nutrientLevel;
                    no_activeTile++;
                }
            }
        }
    }
    return (no_activeTile == 0) ? 0 : (nutrientSum / (float)no_activeTile);
}