#include <stdio.h>

__attribute__((noinline)) float compound(float p, float r) {
    return p * (1.0f + r);
}

int main() {
    float principal = 1e7f;
    float rate = 0.0001f;
    for (int i = 0; i < 10000; i++) {
        principal = compound(principal, rate);
    }
    printf("principal = %.2f\n", principal);
    return 0;
}
