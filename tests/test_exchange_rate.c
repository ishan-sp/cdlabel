#include <stdio.h>

__attribute__((noinline)) float convert(float usd, float rate) {
    return usd * rate;
}

int main() {
    float usd = 1e8f;
    float rate = 1.0000001f;
    float eur = convert(usd, rate);
    printf("eur = %.2f\n", eur);
    return 0;
}
