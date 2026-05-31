#include <stdio.h>

__attribute__((noinline)) float scale(float v, float s) { return v * s; }

int main() {
    float v = 1e10f;
    float scale_factor = 1.0000001f;
    float out = scale(v, scale_factor);
    printf("out = %.2f\n", out);
    return 0;
}
