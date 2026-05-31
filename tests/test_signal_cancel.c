#include <stdio.h>

__attribute__((noinline)) float cancel(float a, float b) { return a - b; }

int main() {
    float signal = 123456.125f;
    float noise = 123456.0f;
    float clean = cancel(signal, noise);
    printf("clean = %.6f\n", clean);
    return 0;
}
