TEST_SRC = src/* test/*.c
CFLAGS = -std=gnu11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -lpthread


.PHONY: test

test:
	$(CC) $(TEST_SRC) $(CFLAGS) -o test.o
	./test.o

test-valgrind:
	make test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./test.o
