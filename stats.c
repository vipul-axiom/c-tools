/* stats: read numbers and print the count, min, max, average, and a
 * sorted copy.
 *
 * Usage: stats [FILE]
 * Reads FILE, or standard input when no FILE (or "-") is given.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

static void usage(FILE *out) {
    fprintf(out, "Usage: stats [FILE]\n");
    fprintf(out, "Read numbers from FILE (or standard input) and print the count,\n");
    fprintf(out, "min, max, average, and a sorted copy.\n");
    fprintf(out, "  -h, --help  show this help and exit\n");
}

int main(int argc, char *argv[]) {
    const char *path = NULL;   /* file name from the command line, if any */
    FILE *in = stdin;          /* where we read from: stdin or the file */
    double *nums = NULL;
    size_t n = 0, cap = 0;
    double x;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(stdout);
            return 0;
        } else if (argv[i][0] == '-' && argv[i][1] != '\0') {
            fprintf(stderr, "stats: unknown option '%s'\n", argv[i]);
            usage(stderr);
            return 1;
        } else if (path != NULL) {
            fprintf(stderr, "stats: only one file can be given\n");
            usage(stderr);
            return 1;
        } else {
            path = argv[i];
        }
    }

    if (path != NULL && strcmp(path, "-") != 0) {
        in = fopen(path, "r");
        if (in == NULL) {
            fprintf(stderr, "stats: %s: %s\n", path, strerror(errno));
            return 1;
        }
    } else {
        fprintf(stderr, "Enter numbers separated by spaces or newlines. Press Ctrl-D to finish.\n");
    }

    errno = 0;  /* so a read error below reports its own cause */
    while (fscanf(in, "%lf", &x) == 1) {
        if (n == cap) {
            cap = cap ? cap * 2 : 8;
            double *next = realloc(nums, cap * sizeof *nums);
            if (!next) {
                fprintf(stderr, "out of memory\n");
                free(nums);
                if (in != stdin) fclose(in);
                return 1;
            }
            nums = next;
        }
        nums[n++] = x;
    }
    /* EOF can also mean a read error (for example, FILE is a directory).
     * Report it instead of printing counts for partial input. */
    if (ferror(in)) {
        int err = errno;
        fprintf(stderr, "stats: %s: %s\n", path != NULL ? path : "-",
                err ? strerror(err) : "read error");
        free(nums);
        if (in != stdin) fclose(in);
        return 1;
    }
    if (in != stdin) fclose(in);

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
