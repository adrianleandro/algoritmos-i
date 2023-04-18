CC = gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LDLIBS=-lm
PROGRAM= generarImagen

TEST_DIMENSIONS= 1024 768 1 #640 480 1
IMGBMP= escena.bmp
IMGPPM= escena.ppm

all: archivo.o imagen.o vector.o main.o arreglo.o objeto.o color.o
	$(CC) archivo.o main.o imagen.o vector.o arreglo.o objeto.o color.o $(CFLAGS) $(LDLIBS) -o $(PROGRAM)

archivo.o: archivo.c archivo.h
	$(CC) archivo.c $(CFLAGS) $(LDLIBS) -c

arreglo.o: arreglo.c arreglo.h
	$(CC) arreglo.c $(CFLAGS) -c

imagen.o: imagen.c imagen.h
	$(CC) imagen.c $(CFLAGS) $(LDLIBS) -c

objeto.o: objeto.c objeto.h
	$(CC) objeto.c $(CFLAGS) $(LDLIBS) -c

color.o: color.c color.h
	$(CC) color.c $(CFLAGS) $(LDLIBS) -c

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
