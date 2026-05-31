#include <stdio.h>

__attribute__((noinline)) float parallel(float r1, float r2) {
    return 1.0f / (1.0f / r1 + 1.0f / r2);
}

int main() {
    float r1 = 1e8f;
    float r2 = 1.0f;
    float r = parallel(r1, r2);
    printf("parallel = %.8f\n", r);
    return 0;
}
