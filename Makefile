CC = cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

all: stats tally wordfreq

stats: stats.c
	$(CC) $(CFLAGS) -o stats stats.c

tally: tally.c
	$(CC) $(CFLAGS) -o tally tally.c

wordfreq: wordfreq.c
	$(CC) $(CFLAGS) -o wordfreq wordfreq.c

test: all
	@status=0; \
	for t in stats tally wordfreq; do \
		if ./$$t < tests/$$t.in 2>/dev/null | diff -u tests/$$t.expected -; then \
			echo "PASS $$t"; \
		else \
			echo "FAIL $$t"; status=1; \
		fi; \
	done; \
	exit $$status

clean:
	rm -f stats tally wordfreq

.PHONY: all test clean
