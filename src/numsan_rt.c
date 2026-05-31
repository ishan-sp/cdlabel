#include <stdio.h>
#include <math.h>

void __numsan_check(float actual, double shadow, int line) {
    double diff = fabs((double)actual - shadow);
    double relative = (fabs(shadow) > 1e-10) ? diff / fabs(shadow) : diff;

    if (relative > 1e-9) {   // lowered from 1e-5 to 1e-9
        if (line > 0) {
            printf("[NUMSAN] WARNING line %d: Catastrophic cancellation detected!\n", line);
        } else {
            printf("[NUMSAN] WARNING line ?: Catastrophic cancellation detected!\n");
        }
        printf("         float result : %.10f\n", (double)actual);
        printf("         double shadow: %.10f\n", shadow);
        printf("         relative diff: %.2e\n", relative);
    }
}
