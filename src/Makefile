CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wpedantic -std=c11
CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

.PHONY: all clean rebuild

all: my_grep

clean: 
	rm -f my_grep my_grep_test

rebuild: clean all

my_grep:
	$(CC) $(CFLAGS) grep.c -o my_grep

test: my_grep
	$(CC) $(FLAGS) ../tests/test_grep.c -o test $(CHECK_FLAGS)
	./test