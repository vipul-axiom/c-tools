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

## License

MIT. See [LICENSE](LICENSE).
