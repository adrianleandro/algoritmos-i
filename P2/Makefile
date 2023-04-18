CC = gcc
CFLAGS=-Wall -std=c99 -pedantic -Werror -g
LDLIBS=-lm
PROGRAM = prueba
VALGRIND= valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s

all: P2.c
	$(CC) P2.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)
	@./$(PROGRAM)

debug: P2.c
	$(CC) P2.c $(CFLAGS) $(LDLIBS) -o $(PROGRAM)
	@$(VALGRIND) ./$(PROGRAM)