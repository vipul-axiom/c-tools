# c-tools

Two small C programs.

## stats

Reads numbers and prints the count, min, max, average, and a sorted copy.

```
gcc -std=c11 -Wall -Wextra -o stats main.c
printf '3\n1\n2\n' | ./stats
```

## tally

Counts lines, words, and characters from text on standard input.

```
gcc -std=c11 -Wall -Wextra -o tally tally.c
printf 'hello world\n' | ./tally
```
