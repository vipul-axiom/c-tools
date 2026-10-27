[![CI](https://github.com/vipul-axiom/c-tools/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/vipul-axiom/c-tools/actions/workflows/ci.yml)

# c-tools

Small C command-line tools.

## Build

You need a C compiler (`cc`/`gcc`/`clang`) and `make`.

```
make          # builds all tools
make stats    # builds just one tool
make test     # builds, then runs the checks (tests/ files plus edge cases)
make clean    # removes the built binaries
```

## Usage

Every tool works the same way:

```
./TOOL FILE      # read FILE
./TOOL           # read standard input
./TOOL -         # also read standard input
./TOOL --help    # show a short usage message (same as -h)
```

When reading standard input, each tool first prints a short prompt to
standard error. When typing input by hand, press Ctrl-D on a new line to
finish. Because the prompt goes to standard error, piped or redirected
output stays clean.

If the file can't be opened or read (for example, it is missing,
unreadable, or a directory), the tool prints an error, prints no counts,
and exits with status 1:

```
$ ./tally missing.txt
tally: missing.txt: No such file or directory
$ echo $?
1
$ ./tally /etc
tally: /etc: Is a directory
```

Empty input is not an error: `stats` prints `No numbers entered.`,
`tally` prints zero counts, and `wordfreq` prints nothing. All exit 0.

Help:

```
$ ./stats --help
Usage: stats [FILE]
Read numbers from FILE (or standard input) and print the count,
min, max, average, and a sorted copy.
  -h, --help  show this help and exit
```

## stats

Reads numbers and prints the count, min, max, average, and a sorted copy.
Source: `stats.c`

```
$ printf '3\n1\n2\n' > numbers.txt
$ ./stats numbers.txt
count:   3
min:     1.0000
max:     3.0000
average: 2.0000
sorted: 1.0000 2.0000 3.0000
$ printf '3\n1\n2\n' | ./stats
Enter numbers separated by spaces or newlines. Press Ctrl-D to finish.
count:   3
min:     1.0000
max:     3.0000
average: 2.0000
sorted: 1.0000 2.0000 3.0000
```

## tally

Counts lines, words, and characters in a file or standard input.
Source: `tally.c`

```
$ printf 'hello world\n' > hello.txt
$ ./tally hello.txt
lines: 1
words: 2
chars: 12
$ printf 'hello world\n' | ./tally
Type or paste text. Press Ctrl-D to finish.
lines: 1
words: 2
chars: 12
```

## wordfreq

Counts how often each word appears in a file or standard input and prints
the words from most to least frequent (ties in alphabetical order).
Words are made of letters, digits, and apostrophes, and are compared
case-insensitively. Source: `wordfreq.c`

```
$ printf 'the cat saw the dog\nThe dog ran\n' > notes.txt
$ ./wordfreq notes.txt
      3 the
      2 dog
      1 cat
      1 ran
      1 saw
$ printf 'the cat saw the dog\nThe dog ran\n' | ./wordfreq
Type or paste text. Press Ctrl-D to finish.
      3 the
      2 dog
      1 cat
      1 ran
      1 saw
```

## License

MIT. See [LICENSE](LICENSE).

## What I learned

- Reading input from either stdin or a file with the same code.
- Writing a Makefile with a test target and running it on every push with GitHub Actions.
- Checking program output against expected files to catch mistakes.

## Next up

- More edge-case tests (empty input, very long lines).
