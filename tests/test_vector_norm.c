#include <math.h>
#include <stdio.h>

__attribute__((noinline)) float norm(float x, float y) {
    return sqrtf(x * x + y * y);
}

int main() {
    float x = 1e20f;
    float y = 3.0f;
    float n = norm(x, y);
    printf("norm = %.2f\n", n);
    return 0;
}
