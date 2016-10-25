#include <cstdlib>
#include <ctime>

#include "Util.hpp"

int Random(int uboundExclusive) {
    int r = rand();
    return r % uboundExclusive;
}

int Random(int lbound, int uboundExclusive) {
    int r = rand();
    r = r % (uboundExclusive - lbound);
    return lbound + r;
}

float Clock() {
    static struct timespec _base;
    static bool firstCall = true;

    if (firstCall) {
        clock_gettime(CLOCK_MONOTONIC, &_base);
        firstCall = false;
    }

    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    float secDiff = (float)(t.tv_sec - _base.tv_sec);
    float msecDiff = (float)((t.tv_nsec - _base.tv_nsec) / 1000000);
    return secDiff + 0.001f * msecDiff;
}


float SineWave(float min, float max, float period, float phase) {
    float ampl = max - min;
    return min + ampl * sin(((Clock() / period) + phase) * 2 * M_PI);
}

bool BlinkFunc(float period) {
    return (int)(Clock() / period) & 1;
}
