#include <stdio.h>

__attribute__((noinline)) float subtract_energy(float e1, float e2) {
    return e1 - e2;
}

int main() {
    float total = 1.0000001e12f;
    float loss = 1.0000000e12f;
    float remaining = subtract_energy(total, loss);
    printf("remaining = %.2f\n", remaining);
    return 0;
}
