#include "helper_methods.h"

/**
 * @brief map_range maps a value from between one range to another
 * @param value the value to map
 * @param min_a the lowest possible value of the current range
 * @param max_a the highest possible value of the current range
 * @param min_b the lowest possible value of the new range
 * @param max_b the highest possible value of the new range
 * @return the value mapped between ranges
 */
float map_range(float value, float min_a, float max_a, float min_b, float max_b){
    float a_span = max_a - min_a;
    float b_span = max_b - min_b;

    float scaled = (value - min_a) / a_span;

    return min_b + (scaled * b_span);
}

float euc_dist(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
