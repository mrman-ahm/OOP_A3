#include "FractalSprawler.h"

FractalSprawler::FractalSprawler(int x, int y)
	: Organism(x, y, 2, 2) {}

void FractalSprawler::grow()
{
	int h_expanded = height + 2;
	int w_expanded = width + 2;

	bool** expanded = new bool* [h_expanded];
	for (int i = 0; i < h_expanded; i++)
	{
		expanded[i] = new bool[w_expanded];
		for (int j = 0; j < w_expanded;j++)
			expanded[i][j] = true;
	}

	if (internalShape)
	{
		for (int i = 0; i < height; i++)
			delete[] internalShape[i];
		delete[] internalShape;
	}

	x -= 1;y -= 1;
	height = h_expanded;
	width = w_expanded;
	internalShape = expanded;
}

void FractalSprawler::shrink()
{
	if (width <= 2 || height <= 2)
	{
		if (internalShape)
		{
			for (int i = 0; i < height; i++)
				delete[] internalShape[i];
			delete[] internalShape;
			internalShape = nullptr;
		}
		width = 0; height = 0;
		return;
	}

	int h_shrinked = height - 2;
	int w_shrinked = width - 2;

	bool** shrinked = new bool* [h_shrinked];
	for (int i = 0; i < h_shrinked; i++)
	{
		shrinked[i] = new bool[w_shrinked];
		for (int j = 0; j < w_shrinked;j++)
			shrinked[i][j] = true;
	}

	if (internalShape)
	{
		for (int i = 0; i < height; i++)
			delete[] internalShape[i];
		delete[] internalShape;
	}

	x += 1;y += 1;
	height = h_shrinked;
	width = w_shrinked;
	internalShape = shrinked;
}

void FractalSprawler::update(Tile** world, int worldWidth, int worldHeight)
{
	int radius = 2 * width;
	int cen_X = x + (width / 2);
	int cen_Y = y + (height / 2);

	float sum_nutrient = 0;
	int no_of_tiles = 0;
	int new_FS_X = -1, new_FS_Y = -1;

	for (int i = cen_Y - radius; i <= cen_Y + radius; i++)
	{
		for (int j = cen_X - radius; j <= cen_X + radius; j++)
		{
			if (i >= 0 && i < worldHeight && j >= 0 && j < worldWidth)
			{
				sum_nutrient += world[i][j].nutrientLevel;
				no_of_tiles++;

				if (new_FS_X == -1 && new_FS_Y == -1 && world[i][j].occupant == nullptr)
				{
					new_FS_X = j; new_FS_Y = i;
				}
			}
		}
	}

	float avg = (no_of_tiles == 0) ? 0 : (sum_nutrient / (float)no_of_tiles);

	if (avg > 70.0f)
	{
		if (x - 1 >= 0 && y - 1 >= 0 &&
			x + width + 1 < worldWidth &&
			y + height + 1 < worldHeight)
			grow();
	}
	else if (avg < 30.0f) shrink();

	if (width == 0 || height == 0) return;

	if (width >= 5 && height >= 5 && new_FS_X != -1 && new_FS_Y != -1)
	{
		world[new_FS_Y][new_FS_X].occupant = new FractalSprawler(new_FS_X, new_FS_Y);

		int old_h = height;
		int old_w = width;

		bool** reseted = new bool* [2];
		for (int i = 0; i < 2; i++) {
			reseted[i] = new bool[2];
			reseted[i][0] = true; reseted[i][1] = true;
		}

		for (int i = 0; i < old_h; i++)
			delete[] internalShape[i];
		delete[] internalShape;

		x += (old_w - 2) / 2;
		y += (old_h - 2) / 2;

		internalShape = reseted;
		width = 2;
		height = 2;
	}
}