#include <stdio.h>

__attribute__((noinline)) float mul(float a, float b) { return a * b; }
__attribute__((noinline)) float make_val(float x) { return x; }

int main() {
    // Large * slightly-off-1.0 — float loses the fractional part
    float r1 = mul(make_val(1e8f), make_val(1.0000001f));
    printf("1e8 * 1.0000001 = %.10f\n", r1);

    // Same but more extreme
    float r2 = mul(make_val(1e10f), make_val(1.0000001f));
    printf("1e10 * 1.0000001 = %.4f\n", r2);

    return 0;
}
