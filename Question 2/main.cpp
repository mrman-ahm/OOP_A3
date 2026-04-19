#include "Aethelgard.h"
#include "string.h"
#include <iostream>

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <scenario_csv> [days]\n";

        return 1;
    }

    const char* csvPath = argv[1];
    int days = (argc >= 3) ? stoi(argv[2]) : 5;

    Aethelgard engine;
    engine.run(csvPath, days);

    return 0;
}
