#include <ctype.h>
#include <stdio.h>

int main(void) {
    int c, lines = 0, words = 0, chars = 0, in_word = 0;

    fprintf(stderr, "Type or paste text. Press Ctrl-D to finish.\n");
    while ((c = getchar()) != EOF) {
        chars++;
        if (c == '\n') lines++;
        if (isspace(c)) in_word = 0;
        else if (!in_word) {
            in_word = 1;
            words++;
        }
    }
    printf("lines: %d\nwords: %d\nchars: %d\n", lines, words, chars);
    return 0;
}
