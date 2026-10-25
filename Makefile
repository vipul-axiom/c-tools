CC = cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

all: stats tally wordfreq

stats: stats.c
	$(CC) $(CFLAGS) -o stats stats.c

tally: tally.c
	$(CC) $(CFLAGS) -o tally tally.c

wordfreq: wordfreq.c
	$(CC) $(CFLAGS) -o wordfreq wordfreq.c

# For each tool, check:
#   1. stdin input matches tests/<tool>.expected
#   2. a file argument gives the same output
#   3. -h and --help print a usage message and exit 0
#   4. a missing file prints "<tool>: <file>: <error>" and exits non-zero
#   5. an unknown option exits non-zero
# Temporary output goes in a directory that is removed at the end.
test: all
	@status=0; tmp=$$(mktemp -d); \
	for t in stats tally wordfreq; do \
		if ./$$t < tests/$$t.in 2>/dev/null | diff -u tests/$$t.expected -; then \
			echo "PASS $$t (stdin)"; \
		else \
			echo "FAIL $$t (stdin)"; status=1; \
		fi; \
		if ./$$t tests/$$t.in 2>/dev/null | diff -u tests/$$t.expected -; then \
			echo "PASS $$t (file argument)"; \
		else \
			echo "FAIL $$t (file argument)"; status=1; \
		fi; \
		for opt in -h --help; do \
			if ./$$t $$opt > $$tmp/help 2>/dev/null && grep -q "^Usage: $$t" $$tmp/help; then \
				echo "PASS $$t ($$opt)"; \
			else \
				echo "FAIL $$t ($$opt)"; status=1; \
			fi; \
		done; \
		if ./$$t tests/no-such-file > $$tmp/out 2> $$tmp/err; then \
			echo "FAIL $$t (missing file: exit status was 0)"; status=1; \
		elif grep -q "^$$t: tests/no-such-file: " $$tmp/err && ! test -s $$tmp/out; then \
			echo "PASS $$t (missing file)"; \
		else \
			echo "FAIL $$t (missing file: wrong message)"; cat $$tmp/err; status=1; \
		fi; \
		if ./$$t --bogus > /dev/null 2>&1; then \
			echo "FAIL $$t (unknown option: exit status was 0)"; status=1; \
		else \
			echo "PASS $$t (unknown option)"; \
		fi; \
	done; \
	rm -rf "$$tmp"; \
	exit $$status

clean:
	rm -f stats tally wordfreq

.PHONY: all test clean
