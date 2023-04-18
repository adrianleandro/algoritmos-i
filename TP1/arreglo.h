#ifndef ARREGLO_H
#define ARREGLO_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct arreglo {
    void **v;
    size_t n;
} arreglo_t;

arreglo_t *arreglo_crear();
bool arreglo_agregar(arreglo_t *a, void *e);
void arreglo_destruir(arreglo_t *arr, void (*eliminar)(void *));

#endif