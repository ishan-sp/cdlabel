#include <stdio.h>

// __attribute__((noinline)) forces the compiler to actually CALL this function
// instead of copying its body into main and precomputing the result
__attribute__((noinline)) float unstable_sum(float a, float b) {
    return a + b;
}

int main() {
    float r1 = unstable_sum(1.0f, 2.0f);
    printf("1.0 + 2.0 = %f\n", r1);

    float r2 = unstable_sum(1e8f, 1.0f);
    printf("1e8 + 1.0 = %.1f\n", r2);

    float r3 = unstable_sum(1e10f, 1.0f);
    printf("1e10 + 1.0 = %.1f\n", r3);

    return 0;
}
