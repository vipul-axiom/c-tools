/* tally: count lines, words, and characters.
 *
 * Usage: tally [FILE]
 * Reads FILE, or standard input when no FILE (or "-") is given.
 */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static void usage(FILE *out) {
    fprintf(out, "Usage: tally [FILE]\n");
    fprintf(out, "Count lines, words, and characters in FILE (or standard input).\n");
    fprintf(out, "  -h, --help  show this help and exit\n");
}

int main(int argc, char *argv[]) {
    const char *path = NULL;   /* file name from the command line, if any */
    FILE *in = stdin;          /* where we read from: stdin or the file */
    int c, lines = 0, words = 0, chars = 0, in_word = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(stdout);
            return 0;
        } else if (argv[i][0] == '-' && argv[i][1] != '\0') {
            fprintf(stderr, "tally: unknown option '%s'\n", argv[i]);
            usage(stderr);
            return 1;
        } else if (path != NULL) {
            fprintf(stderr, "tally: only one file can be given\n");
            usage(stderr);
            return 1;
        } else {
            path = argv[i];
        }
    }

    if (path != NULL && strcmp(path, "-") != 0) {
        in = fopen(path, "r");
        if (in == NULL) {
            fprintf(stderr, "tally: %s: %s\n", path, strerror(errno));
            return 1;
        }
    } else {
        fprintf(stderr, "Type or paste text. Press Ctrl-D to finish.\n");
    }

    while ((c = getc(in)) != EOF) {
        chars++;
        if (c == '\n') lines++;
        if (isspace(c)) in_word = 0;
        else if (!in_word) {
            in_word = 1;
            words++;
        }
    }
    if (in != stdin) fclose(in);

    printf("lines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
    return 0;
}
