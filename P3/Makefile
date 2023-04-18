CC = gcc
CFLAGS=-Wall -std=c99 -pedantic -Werror -g
LDLIBS=-lm
PROGRAM = prueba
VALGRIND= valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s

p3: P3-3.c
	$(CC) P3-3.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)

all: P3.c
	$(CC) P3.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)

debug: P3.c
	$(CC) P3.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)
	$(VALGRIND) ./$(PROGRAM)