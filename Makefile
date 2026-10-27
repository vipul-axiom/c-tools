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
#   6. empty input gives the documented output and exits 0
#   7. a very long line (100000+ characters, generated here) gives the
#      right counts; for wordfreq it includes a 100000-letter word
#   8. a directory argument prints "<tool>: <dir>: <error>", no counts,
#      and exits non-zero
#   9. an unreadable file (chmod 000) prints an error and exits non-zero
#      (SKIP when running as root, since root can read it anyway)
# Temporary files go in a directory that is removed at the end.
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
		case $$t in \
		stats) printf 'No numbers entered.\n' > $$tmp/empty.expected ;; \
		tally) printf 'lines: 0\nwords: 0\nchars: 0\n' > $$tmp/empty.expected ;; \
		wordfreq) : > $$tmp/empty.expected ;; \
		esac; \
		if ./$$t < /dev/null 2>/dev/null > $$tmp/out && diff -u $$tmp/empty.expected $$tmp/out; then \
			echo "PASS $$t (empty input)"; \
		else \
			echo "FAIL $$t (empty input)"; status=1; \
		fi; \
		case $$t in \
		stats) awk 'BEGIN { n = 30000; \
			for (i = 1; i <= n; i++) printf "%s%d", (i > 1 ? " " : ""), n + 1 - i; print "" }' > $$tmp/long.in; \
			awk 'BEGIN { n = 30000; \
			printf "count:   %d\nmin:     %.4f\nmax:     %.4f\naverage: %.4f\nsorted: ", n, 1, n, (n + 1) / 2; \
			for (i = 1; i <= n; i++) printf "%s%.4f", (i > 1 ? " " : ""), i; print "" }' > $$tmp/long.expected ;; \
		tally) awk 'BEGIN { for (i = 0; i < 50000; i++) printf "ab "; print "" }' > $$tmp/long.in; \
			printf 'lines: 1\nwords: 50000\nchars: 150001\n' > $$tmp/long.expected ;; \
		wordfreq) awk 'BEGIN { w = ""; for (i = 0; i < 100000; i++) w = w (i % 2 ? "x" : "X"); \
			printf "%s ", w; for (i = 0; i < 30000; i++) printf "the "; print w }' > $$tmp/long.in; \
			awk 'BEGIN { w = ""; for (i = 0; i < 100000; i++) w = w "x"; \
			printf "%7d the\n%7d %s\n", 30000, 2, w }' > $$tmp/long.expected ;; \
		esac; \
		if ./$$t $$tmp/long.in > $$tmp/out 2>/dev/null && cmp -s $$tmp/long.expected $$tmp/out; then \
			echo "PASS $$t (very long line)"; \
		else \
			echo "FAIL $$t (very long line)"; status=1; \
		fi; \
		if ./$$t tests > $$tmp/out 2> $$tmp/err; then \
			echo "FAIL $$t (directory: exit status was 0)"; status=1; \
		elif grep -q "^$$t: tests: " $$tmp/err && ! test -s $$tmp/out; then \
			echo "PASS $$t (directory)"; \
		else \
			echo "FAIL $$t (directory: wrong message or output)"; cat $$tmp/err; status=1; \
		fi; \
		if [ "$$(id -u)" -eq 0 ]; then \
			echo "SKIP $$t (unreadable file: running as root)"; \
		else \
			echo 1 > $$tmp/unreadable; chmod 000 $$tmp/unreadable; \
			if ./$$t $$tmp/unreadable > $$tmp/out 2> $$tmp/err; then \
				echo "FAIL $$t (unreadable file: exit status was 0)"; status=1; \
			elif grep -q "^$$t: $$tmp/unreadable: " $$tmp/err && ! test -s $$tmp/out; then \
				echo "PASS $$t (unreadable file)"; \
			else \
				echo "FAIL $$t (unreadable file: wrong message or output)"; cat $$tmp/err; status=1; \
			fi; \
			rm -f $$tmp/unreadable; \
		fi; \
	done; \
	rm -rf "$$tmp"; \
	exit $$status

clean:
	rm -f stats tally wordfreq

.PHONY: all test clean
