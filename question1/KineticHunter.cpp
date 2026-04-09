#include "KineticHunter.h"
#include "FractalSprawler.h"
#include <cstdlib>

KineticHunter::KineticHunter(int x, int y)
	:Organism(x, y, 3, 3) {}

void KineticHunter::update(Tile** world, int worldWidth, int worldHeight)
{
	int radius = 4 * width;
	int cen_X = x + (width / 2);
	int cen_Y = y + (height / 2);

	int target_X = -1, target_Y = -1;
	bool targetFound = false;

	for (int i = cen_Y - radius; i <= cen_Y + radius && !targetFound; i++)
	{
		for (int j = cen_X - radius; j <= cen_X + radius && !targetFound; j++)
		{
			if (i >= 0 && i < worldHeight && j >= 0 && j < worldWidth)
			{
				if (world[i][j].occupant != nullptr && world[i][j].occupant->getType() == 1)
				{
					target_X = j;
					target_Y = i;
					targetFound = true;
				}
			}
		}
	}

	if (targetFound)
	{
		if (target_X > x) x++;
		else if (target_X < x) x--;

		if (target_Y > y) y++;
		else if (target_Y < y) y--;
	}
	else
	{
		// move in a random direction
		int dir = rand() % 4;
		if (dir == 0 && x + 1 < worldWidth)  x++;
		else if (dir == 1 && x - 1 >= 0)     x--;
		else if (dir == 2 && y + 1 < worldHeight) y++;
		else if (dir == 3 && y - 1 >= 0)     y--;
	}

	for (int ver_body = 0; ver_body < height; ver_body++)
	{
		for (int hor_body = 0; hor_body < width; hor_body++)
		{
			if (internalShape[ver_body][hor_body])
			{
				int world_Y = y + ver_body;
				int world_X = x + hor_body;

				if (world_Y >= 0 && world_Y < worldHeight && world_X >= 0 && world_X < worldWidth)
				{
					if (world[world_Y][world_X].occupant != nullptr && world[world_Y][world_X].occupant->getType() == 1)
					{
						world[world_Y][world_X].nutrientLevel -= 20.0f;
						if (world[world_Y][world_X].nutrientLevel < 0)
							world[world_Y][world_X].nutrientLevel = 0;

						world[world_Y][world_X].toxicity += 10.0f;
						if (world[world_Y][world_X].toxicity > 100.0f)
							world[world_Y][world_X].toxicity = 100.0f;

						FractalSprawler* fs = (FractalSprawler*)world[world_Y][world_X].occupant;
						if (fs->getW() > 0 && fs->getH() > 0)   // ADD THIS CHECK
							fs->shrink();
					}
				}
			}
		}
	}
}