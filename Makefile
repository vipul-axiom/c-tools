CC = cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

all: stats tally

stats: stats.c
	$(CC) $(CFLAGS) -o stats stats.c

tally: tally.c
	$(CC) $(CFLAGS) -o tally tally.c

clean:
	rm -f stats tally

.PHONY: all clean
