/* wordfreq: count how often each word appears in text from stdin.
 *
 * A word is a run of letters, digits, and apostrophes. Words are
 * lowercased, so "The" and "the" count as the same word. Apostrophes at
 * the start or end of a word are dropped, so 'quoted' becomes quoted.
 *
 * Output: one line per word, most frequent first; ties are sorted
 * alphabetically.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entry {
    char *word;
    size_t count;
};

static struct entry *entries = NULL;
static size_t n_entries = 0, cap_entries = 0;

static int is_word_char(int c) {
    return isalnum(c) || c == '\'';
}

/* Make a heap copy of s. Returns NULL if out of memory. */
static char *copy_string(const char *s) {
    size_t len = strlen(s);
    char *p = malloc(len + 1);
    if (p) memcpy(p, s, len + 1);
    return p;
}

/* Add one occurrence of word. Returns 0 on success, -1 if out of memory. */
static int add_word(const char *word) {
    /* Linear search keeps the code simple; fine for everyday text. */
    for (size_t i = 0; i < n_entries; i++) {
        if (strcmp(entries[i].word, word) == 0) {
            entries[i].count++;
            return 0;
        }
    }
    if (n_entries == cap_entries) {
        size_t new_cap = cap_entries ? cap_entries * 2 : 64;
        struct entry *next = realloc(entries, new_cap * sizeof *entries);
        if (!next) return -1;
        entries = next;
        cap_entries = new_cap;
    }
    char *copy = copy_string(word);
    if (!copy) return -1;
    entries[n_entries].word = copy;
    entries[n_entries].count = 1;
    n_entries++;
    return 0;
}

/* Sort by count (highest first), then alphabetically. */
static int cmp_entry(const void *a, const void *b) {
    const struct entry *x = a;
    const struct entry *y = b;
    if (x->count != y->count) return x->count < y->count ? 1 : -1;
    return strcmp(x->word, y->word);
}

static void free_all(char *buf) {
    for (size_t i = 0; i < n_entries; i++) free(entries[i].word);
    free(entries);
    free(buf);
}

int main(void) {
    char *buf = NULL;          /* current word being read */
    size_t len = 0, cap = 0;
    int c;

    fprintf(stderr, "Type or paste text. Press Ctrl-D to finish.\n");

    do {
        c = getchar();
        if (c != EOF && is_word_char(c)) {
            if (len + 1 >= cap) {  /* leave room for the '\0' */
                size_t new_cap = cap ? cap * 2 : 32;
                char *next = realloc(buf, new_cap);
                if (!next) {
                    fprintf(stderr, "out of memory\n");
                    free_all(buf);
                    return 1;
                }
                buf = next;
                cap = new_cap;
            }
            buf[len++] = (char)tolower(c);
        } else if (len > 0) {
            /* End of a word: trim apostrophes at both ends. */
            buf[len] = '\0';
            char *start = buf;
            while (*start == '\'') start++;
            char *end = buf + len;
            while (end > start && end[-1] == '\'') end--;
            *end = '\0';

            if (*start != '\0' && add_word(start) != 0) {
                fprintf(stderr, "out of memory\n");
                free_all(buf);
                return 1;
            }
            len = 0;
        }
    } while (c != EOF);

    qsort(entries, n_entries, sizeof *entries, cmp_entry);
    for (size_t i = 0; i < n_entries; i++) {
        printf("%7zu %s\n", entries[i].count, entries[i].word);
    }

    free_all(buf);
    return 0;
}
