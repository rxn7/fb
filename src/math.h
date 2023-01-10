#pragma once

#define RADIANS_TO_DEGREES 57.2957795131f

static inline float lerpf(float a, float b, float v) {
    return a + v * (b - a);
}