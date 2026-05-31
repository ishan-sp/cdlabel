#include <stdio.h>

__attribute__((noinline)) float add_fee(float amount, float fee) {
    return amount + fee;
}

int main() {
    float balance = 1e9f;
    float fee = 0.01f;
    float new_balance = add_fee(balance, fee);
    printf("new balance = %.2f\n", new_balance);
    return 0;
}
