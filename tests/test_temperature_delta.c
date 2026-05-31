#include <stdio.h>

__attribute__((noinline)) float diff(float a, float b) { return a - b; }

int main() {
    float temp_reading = 273.15002f;
    float reference = 273.15f;
    float delta = diff(temp_reading, reference);
    printf("delta = %.8f\n", delta);
    return 0;
}
