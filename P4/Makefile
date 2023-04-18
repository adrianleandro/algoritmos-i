CC = gcc
CFLAGS=-Wall -std=c99 -pedantic -Werror -g
LDLIBS=-lm
PROGRAM = prueba
VALGRIND= valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s

all: P4.c
	$(CC) P4.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)

debug: P4.c
	$(CC) P4.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)
	$(VALGRIND) ./$(PROGRAM)