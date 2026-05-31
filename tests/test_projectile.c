#include <math.h>
#include <stdio.h>

__attribute__((noinline)) float compute_height(float v, float t) {
    return v * t - 0.5f * 9.81f * t * t;
}

int main() {
    float v = 1000.0f;
    float t = 10.0f;
    float h = compute_height(v, t);
    printf("height = %.4f\n", h);
    return 0;
}
