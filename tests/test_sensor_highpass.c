#include <stdio.h>

__attribute__((noinline)) float highpass(float x, float y) { return x - y; }

int main() {
    float gyro = 1.0000001e6f;
    float baseline = 1.0000000e6f;
    float delta = highpass(gyro, baseline);
    printf("delta = %.6f\n", delta);
    return 0;
}
