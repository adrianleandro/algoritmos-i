CC = gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LDLIBS=-lm
VALGRIND= valgrind --leak-check=full --track-origins=yes --show-reachable=yes

all: EJ3.c
	$(CC) EJ3.c $(CFLAGS) $(LDLIBS) -o generarImagen
	@./generarImagen > EJ3.ppm

debug: EJ3.c
	$(CC) EJ3.c $(CFLAGS) $(LDLIBS) -o generarImagen
	@$(VALGRIND) ./generarImagen > EJ3.ppm