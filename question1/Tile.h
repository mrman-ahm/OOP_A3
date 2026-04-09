#pragma once

class Organism;

// Done

struct Tile {
	float nutrientLevel;
	float toxicity;
	Organism* occupant;
};