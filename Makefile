CC = clang
CFLAGS = -Wall -Werror -g
make:
	$(CC) -o mallocTest $(CFLAGS) mallocTest.c
