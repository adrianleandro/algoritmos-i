CC = gcc
CFLAGS=-Wall -std=c99 -pedantic
LDLIBS=-lm

all: EJ1.c
	$(CC) EJ1.c $(CFLAGS) $(LDLIBS) -o generarImagen
	@./generarImagen > EJ1.pbm
	@xdg-open EJ1.pbm