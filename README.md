# c-tools

Small C command-line tools.

## Build

You need a C compiler (`cc`/`gcc`/`clang`) and `make`.

```
make          # builds all tools
make stats    # builds just one tool
make clean    # removes the built binaries
```

Each tool reads from standard input. When typing input by hand, press
Ctrl-D on a new line to finish. Prompts go to standard error, so piped
output stays clean.

## stats

Reads numbers and prints the count, min, max, average, and a sorted copy.
Source: `stats.c`

```
printf '3\n1\n2\n' | ./stats
```

## tally

Counts lines, words, and characters from text on standard input.
Source: `tally.c`

```
printf 'hello world\n' | ./tally
```

## wordfreq

Counts how often each word appears in text on standard input and prints
the words from most to least frequent (ties in alphabetical order).
Words are made of letters, digits, and apostrophes, and are compared
case-insensitively. Source: `wordfreq.c`

```
$ printf 'the cat saw the dog\nThe dog ran\n' | ./wordfreq
      3 the
      2 dog
      1 cat
      1 ran
      1 saw
```

You can also count a whole file: `./wordfreq < notes.txt`

## License

MIT. See [LICENSE](LICENSE).
