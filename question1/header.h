#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==========================================
// TILE STRUCT
// ==========================================
class Organism;

struct Tile {
    float nutrientLevel;
    float toxicity;
    Organism* occupant;
};

// ==========================================
// ORGANISM BASE CLASS
// ==========================================
class Organism {
protected:
    int x, y;
    int width, height;
    bool** internalShape;

public:
    Organism(int x, int y, int width, int height);
    virtual ~Organism();

    virtual void update(Tile** world, int worldWidth, int worldHeight) = 0;
    float getAverageNutrients(Tile** world, int worldWidth, int worldHeight);

    int getX() { return x; }
    int getY() { return y; }
    int getW() { return width; }
    int getH() { return height; }
    bool isAliveAt(int r, int c) { return internalShape[r][c]; }
    virtual int getType() = 0;
};

// ==========================================
// FRACTAL SPRAWLER (PRODUCER)
// ==========================================
class FractalSprawler : public Organism {
private:
    void grow();

public:
    FractalSprawler(int x, int y);
    void shrink();
    void update(Tile** world, int worldWidth, int worldHeight) override;
    int getType() override { return 1; }
};

// ==========================================
// KINETIC HUNTER (CONSUMER)
// ==========================================
class KineticHunter : public Organism {
public:
    KineticHunter(int x, int y);
    void update(Tile** world, int worldWidth, int worldHeight) override;
    int getType() override { return 2; }
};

// ==========================================
// WORLD CLASS
// ==========================================
class World {
private:
    int width, height;
    Tile** grid;
    Organism** population;
    int total_population;
    int limit_population;

public:
    World(int height, int width);
    ~World();

    void runIteration();
    void display();
    void app_Organism(Organism* newOrganism);
};

// ==========================================
// ORGANISM IMPLEMENTATIONS
// ==========================================
Organism::Organism(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {
    internalShape = new bool* [height];
    for (int i = 0; i < height; i++) {
        internalShape[i] = new bool[width];
        for (int j = 0; j < width; j++)
            internalShape[i][j] = true;
    }
}

Organism::~Organism() {
    if (internalShape) {
        for (int i = 0; i < height; i++)
            delete[] internalShape[i];
        delete[] internalShape;
        internalShape = nullptr;
    }
}

float Organism::getAverageNutrients(Tile** world, int worldWidth, int worldHeight) {
    float nutrientSum = 0.0f;
    int no_activeTile = 0;

    for (int ver_body = 0; ver_body < height; ver_body++) {
        for (int hor_body = 0; hor_body < width; hor_body++) {
            if (internalShape[ver_body][hor_body]) {
                int wy = y + ver_body;
                int wx = x + hor_body;
                if (wy >= 0 && wy < worldHeight && wx >= 0 && wx < worldWidth) {
                    nutrientSum += world[wy][wx].nutrientLevel;
                    no_activeTile++;
                }
            }
        }
    }
    return (no_activeTile == 0) ? 0 : (nutrientSum / (float)no_activeTile);
}

// ==========================================
// FRACTAL SPRAWLER IMPLEMENTATIONS
// ==========================================
FractalSprawler::FractalSprawler(int x, int y)
    : Organism(x, y, 2, 2) {
}

void FractalSprawler::grow() {
    int h_expanded = height + 2;
    int w_expanded = width + 2;

    bool** expanded = new bool* [h_expanded];
    for (int i = 0; i < h_expanded; i++) {
        expanded[i] = new bool[w_expanded];
        for (int j = 0; j < w_expanded; j++)
            expanded[i][j] = true;
    }

    if (internalShape) {
        for (int i = 0; i < height; i++)
            delete[] internalShape[i];
        delete[] internalShape;
    }

    x -= 1; y -= 1;
    height = h_expanded;
    width = w_expanded;
    internalShape = expanded;
}

void FractalSprawler::shrink() {
    if (width <= 2 || height <= 2) {
        if (internalShape) {
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
    for (int i = 0; i < h_shrinked; i++) {
        shrinked[i] = new bool[w_shrinked];
        for (int j = 0; j < w_shrinked; j++)
            shrinked[i][j] = true;
    }

    if (internalShape) {
        for (int i = 0; i < height; i++)
            delete[] internalShape[i];
        delete[] internalShape;
    }

    x += 1; y += 1;
    height = h_shrinked;
    width = w_shrinked;
    internalShape = shrinked;
}

void FractalSprawler::update(Tile** world, int worldWidth, int worldHeight) {
    int radius = 2 * width;
    int cen_X = x + (width / 2);
    int cen_Y = y + (height / 2);

    float sum_nutrient = 0;
    int no_of_tiles = 0;
    int new_FS_X = -1, new_FS_Y = -1;

    for (int i = cen_Y - radius; i <= cen_Y + radius; i++) {
        for (int j = cen_X - radius; j <= cen_X + radius; j++) {
            if (i >= 0 && i < worldHeight && j >= 0 && j < worldWidth) {
                sum_nutrient += world[i][j].nutrientLevel;
                no_of_tiles++;

                if (new_FS_X == -1 && new_FS_Y == -1 && world[i][j].occupant == nullptr) {
                    new_FS_X = j; new_FS_Y = i;
                }
            }
        }
    }

    float avg = (no_of_tiles == 0) ? 0 : (sum_nutrient / (float)no_of_tiles);

    if (avg > 70.0f) {
        if (x - 1 >= 0 && y - 1 >= 0 &&
            x + width + 1 < worldWidth &&
            y + height + 1 < worldHeight)
            grow();
    }
    else if (avg < 30.0f) shrink();

    if (width == 0 || height == 0) return;

    if (width >= 5 && height >= 5 && new_FS_X != -1 && new_FS_Y != -1) {
        // Create new offspring
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

// ==========================================
// KINETIC HUNTER IMPLEMENTATIONS
// ==========================================
KineticHunter::KineticHunter(int x, int y)
    : Organism(x, y, 3, 3) {
}

void KineticHunter::update(Tile** world, int worldWidth, int worldHeight) {
    int radius = 4 * width;
    int cen_X = x + (width / 2);
    int cen_Y = y + (height / 2);

    int target_X = -1, target_Y = -1;
    bool targetFound = false;

    for (int i = cen_Y - radius; i <= cen_Y + radius && !targetFound; i++) {
        for (int j = cen_X - radius; j <= cen_X + radius && !targetFound; j++) {
            if (i >= 0 && i < worldHeight && j >= 0 && j < worldWidth) {
                if (world[i][j].occupant != nullptr && world[i][j].occupant->getType() == 1) {
                    target_X = j;
                    target_Y = i;
                    targetFound = true;
                }
            }
        }
    }

    if (targetFound) {
        if (target_X > x) x++;
        else if (target_X < x) x--;

        if (target_Y > y) y++;
        else if (target_Y < y) y--;
    }
    else {
        int dir = rand() % 4;
        if (dir == 0 && x + width < worldWidth) x++;
        else if (dir == 1 && x - 1 >= 0) x--;
        else if (dir == 2 && y + height < worldHeight) y++;
        else if (dir == 3 && y - 1 >= 0) y--;
    }

    for (int ver_body = 0; ver_body < height; ver_body++) {
        for (int hor_body = 0; hor_body < width; hor_body++) {
            if (internalShape[ver_body][hor_body]) {
                int world_Y = y + ver_body;
                int world_X = x + hor_body;

                if (world_Y >= 0 && world_Y < worldHeight && world_X >= 0 && world_X < worldWidth) {
                    if (world[world_Y][world_X].occupant != nullptr && world[world_Y][world_X].occupant->getType() == 1) {
                        world[world_Y][world_X].nutrientLevel -= 20.0f;
                        if (world[world_Y][world_X].nutrientLevel < 0)
                            world[world_Y][world_X].nutrientLevel = 0;

                        world[world_Y][world_X].toxicity += 10.0f;
                        if (world[world_Y][world_X].toxicity > 100.0f)
                            world[world_Y][world_X].toxicity = 100.0f;

                        FractalSprawler* fs = (FractalSprawler*)world[world_Y][world_X].occupant;
                        if (fs->getW() > 0 && fs->getH() > 0)
                            fs->shrink();
                    }
                }
            }
        }
    }
}

// ==========================================
// WORLD IMPLEMENTATIONS
// ==========================================
World::World(int height, int width)
    : height(height), width(width), total_population(0), limit_population(10) {
    population = new Organism * [limit_population];
    grid = new Tile * [height];
    for (int i = 0; i < height; i++) {
        grid[i] = new Tile[width];
        for (int j = 0; j < width; j++) {
            grid[i][j].nutrientLevel = (float)(rand() % 101);
            grid[i][j].toxicity = 0;
            grid[i][j].occupant = nullptr;
        }
    }
}

World::~World() {
    for (int i = 0; i < total_population; i++)
        delete population[i];
    delete[] population;

    for (int i = 0; i < height; i++)
        delete[] grid[i];
    delete[] grid;
}

void World::app_Organism(Organism* newOrganism) {
    if (total_population >= limit_population) {
        limit_population *= 2;
        Organism** temp = new Organism * [limit_population];
        for (int i = 0; i < total_population; i++)
            temp[i] = population[i];
        delete[] population;
        population = temp;
    }
    population[total_population++] = newOrganism;
}

void World::runIteration() {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            grid[i][j].occupant = nullptr;

    for (int i = 0; i < total_population; i++) {
        Organism* org = population[i];
        for (int r = 0; r < org->getH(); r++) {
            for (int c = 0; c < org->getW(); c++) {
                if (org->isAliveAt(r, c)) {
                    int wy = org->getY() + r;
                    int wx = org->getX() + c;
                    if (wy >= 0 && wy < height && wx >= 0 && wx < width)
                        grid[wy][wx].occupant = org;
                }
            }
        }
    }

    for (int i = 0; i < total_population; i++)
        population[i]->update(grid, width, height);

    int alive = 0;
    for (int i = 0; i < total_population; i++) {
        if (population[i]->getW() == 0 || population[i]->getH() == 0) {
            for (int r = 0; r < height; r++)
                for (int c = 0; c < width; c++)
                    if (grid[r][c].occupant == population[i])
                        grid[r][c].occupant = nullptr;
            delete population[i];
        }
        else {
            population[alive++] = population[i];
        }
    }
    total_population = alive;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j].occupant == nullptr) continue;
            bool found = false;
            for (int k = 0; k < total_population; k++) {
                if (population[k] == grid[i][j].occupant) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                app_Organism(grid[i][j].occupant);
                // Clear pointer from grid temporarily to avoid multiple additions 
                // of the same multi-tile organism offspring.
                Organism* newOrg = grid[i][j].occupant;
                for (int r = 0; r < height; r++)
                    for (int c = 0; c < width; c++)
                        if (grid[r][c].occupant == newOrg)
                            grid[r][c].occupant = nullptr;
            }
        }
    }

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            grid[i][j].occupant = nullptr;

    for (int i = 0; i < total_population; i++) {
        Organism* org = population[i];
        for (int r = 0; r < org->getH(); r++) {
            for (int c = 0; c < org->getW(); c++) {
                if (org->isAliveAt(r, c)) {
                    int wy = org->getY() + r;
                    int wx = org->getX() + c;
                    if (wy >= 0 && wy < height && wx >= 0 && wx < width)
                        grid[wy][wx].occupant = org;
                }
            }
        }
    }

    display();
}

void World::display() {
    float totalToxicity = 0.0f;
    unsigned char block = 219;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            totalToxicity += grid[i][j].toxicity;

            if (grid[i][j].occupant == nullptr) {
                cout << "\033[30m" << block << block << "\033[0m";
            }
            else if (grid[i][j].occupant->getType() == 1) {
                cout << "\033[32m" << block << block << "\033[0m";
            }
            else {
                cout << "\033[31m" << block << block << "\033[0m";
            }
        }
        cout << "\n";
    }

    cout << "Total Population: " << total_population << "\n";
    cout << "Average World Toxicity: " << totalToxicity / (float)(width * height) << "\n\n";
}

