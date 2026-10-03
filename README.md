# c-tools

A small C program that reads numbers and prints the count, min, max, average, and a sorted copy.

## Build

```
gcc -std=c11 -Wall -Wextra -o stats main.c
```

## Run

```
./stats
```

Type one number per line, then press Ctrl-D (or Ctrl-Z on Windows).

Example:

```
$ printf '3\n1\n2\n' | ./stats
count:   3
min:     1.0000
max:     3.0000
average: 2.0000
sorted: 1.0000 2.0000 3.0000
```
