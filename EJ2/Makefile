CC = gcc
CFLAGS=-Wall -std=c99 -pedantic
LDLIBS=-lm

all: EJ2.c
	$(CC) EJ2.c $(CFLAGS) $(LDLIBS) -o generarImagen
	@rm EJ2.png
	@rm EJ2.pgm
	@./generarImagen > EJ2.pgm
	@convert EJ2.pgm EJ2.png
	@xdg-open EJ2.png