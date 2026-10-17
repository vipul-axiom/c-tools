CC = cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

all: stats tally wordfreq

stats: stats.c
	$(CC) $(CFLAGS) -o stats stats.c

tally: tally.c
	$(CC) $(CFLAGS) -o tally tally.c

wordfreq: wordfreq.c
	$(CC) $(CFLAGS) -o wordfreq wordfreq.c

clean:
	rm -f stats tally wordfreq

.PHONY: all clean
