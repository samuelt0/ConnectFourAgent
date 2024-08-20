#include "Heuristic.h"
#include <vector>

int scoreSet(const std::vector<unsigned int>& v, unsigned int p) {
    int score = 0;
    int count = 0;

    for (int i = 0; i < 4; i++) {
        if (v[i] == p) { count++; }
        else if (v[i] == 0) { count = 0; }
        else { count = -1; break; }
    }

    if (count == 4) { score += 1000; }
    else if (count == 3) { score += 10; }
    else if (count == 2) { score += 1; }

    return score;
}

int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
    return g - b + z;
}
