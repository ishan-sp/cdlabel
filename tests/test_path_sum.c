#include <stdio.h>

__attribute__((noinline)) float step(float total, float inc) {
    return total + inc;
}

int main() {
    float total = 0.0f;
    for (int i = 0; i < 1000000; i++) {
        total = step(total, 0.0001f);
    }
    printf("total = %.6f\n", total);
    return 0;
}
