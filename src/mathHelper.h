#pragma once

#include <stdlib.h>
#define RADIANS_TO_DEGREES 57.2957795131f

static inline float lerpf(float a, float b, float v) {
    return a + v * (b - a);
}

static inline float rand_range(float min, float max) {
    float r = (float)rand() / (float)RAND_MAX;
    return r * (max-min) + min;
}