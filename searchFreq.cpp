#include <cmath>

float searchFreq(float note, float freqs[60]) {
    float closest = freqs[0];
    float min_diff = abs(note - freqs[0]);

    for (int i = 0; i<60; i++) {
        float diff = abs(freqs[i]-note);
        if (diff<min_diff) {
            min_diff = diff;
            closest = freqs[i];
        }
        if (min_diff == 0) {
            break;
        }
    }
    return closest;
}