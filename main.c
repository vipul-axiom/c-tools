#include <stdio.h>
#include <stdlib.h>

static int cmp(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

int main(void) {
    double *nums = NULL;
    size_t n = 0, cap = 0;
    double x;

    printf("Enter numbers, one per line. Empty line to finish.\n");
    while (scanf("%lf", &x) == 1) {
        if (n == cap) {
            cap = cap ? cap * 2 : 8;
            double *next = realloc(nums, cap * sizeof *nums);
            if (!next) {
                fprintf(stderr, "out of memory\n");
                free(nums);
                return 1;
            }
            nums = next;
        }
        nums[n++] = x;
    }

    if (n == 0) {
        printf("No numbers entered.\n");
        return 0;
    }

    double min = nums[0], max = nums[0], sum = 0;
    for (size_t i = 0; i < n; i++) {
        if (nums[i] < min) min = nums[i];
        if (nums[i] > max) max = nums[i];
        sum += nums[i];
    }
    qsort(nums, n, sizeof *nums, cmp);

    printf("count:   %zu\n", n);
    printf("min:     %.4f\n", min);
    printf("max:     %.4f\n", max);
    printf("average: %.4f\n", sum / n);
    printf("sorted: ");
    for (size_t i = 0; i < n; i++) {
        printf("%s%.4f", i ? " " : "", nums[i]);
    }
    printf("\n");
    free(nums);
    return 0;
}
