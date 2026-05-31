#include <math.h>
#include <stdio.h>

__attribute__((noinline)) void solve_naive(float a, float b, float c,
                                           float *x1, float *x2) {
    float disc = b * b - 4.0f * a * c;
    float sqrt_disc = sqrtf(disc);
    *x1 = (-b + sqrt_disc) / (2.0f * a);
    *x2 = (-b - sqrt_disc) / (2.0f * a);
}

__attribute__((noinline)) void solve_stable(float a, float b, float c,
                                            float *x1, float *x2) {
    float disc = b * b - 4.0f * a * c;
    float sqrt_disc = sqrtf(disc);
    float q = -0.5f * (b + copysignf(sqrt_disc, b));
    *x1 = q / a;
    *x2 = c / q;
}

int main() {
    float x1 = 0.0f;
    float x2 = 0.0f;

    solve_naive(1.0f, -5.0f, 6.0f, &x1, &x2);
    printf("naive: x^2 - 5x + 6 => x1=%f x2=%f\n", x1, x2);

    solve_naive(1.0f, 1e8f, 1.0f, &x1, &x2);
    printf("naive: x^2 + 1e8x + 1 => x1=%f x2=%f\n", x1, x2);

    solve_stable(1.0f, 1e8f, 1.0f, &x1, &x2);
    printf("stable: x^2 + 1e8x + 1 => x1=%f x2=%f\n", x1, x2);

    return 0;
}
