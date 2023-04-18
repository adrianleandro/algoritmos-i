CC = gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LDLIBS=-lm
PROGRAM= generarImagen

TEST_DIMENSIONS= 640 480
IMGBMP= mandelbrot.bmp
IMGPPM= mandelbrot.ppm

all: archivo.o imagen.o vector.o main.o
	$(CC) archivo.o main.o imagen.o vector.o $(CFLAGS) $(LDLIBS) -o $(PROGRAM)

archivo.o: archivo.c archivo.h
	$(CC) archivo.c $(CFLAGS) $(LDLIBS) -c

imagen.o: imagen.c imagen.h
	$(CC) imagen.c $(CFLAGS) $(LDLIBS) -c

vector.o: vector.c vector.h
	$(CC) vector.c $(CFLAGS) $(LDLIBS) -c

main.o: main.c
	$(CC) main.c $(CFLAGS) $(LDLIBS) -c

ppm: all
	./$(PROGRAM) $(TEST_DIMENSIONS) $(IMGPPM)

bmp: all
	./$(PROGRAM) $(TEST_DIMENSIONS) $(IMGBMP)

clean:
	rm -vf *.o generarImagen *.bmp *.ppm