#include "arreglo.h"

arreglo_t *arreglo_crear(){
    arreglo_t *arr = malloc(sizeof(arreglo_t));
    if(!arr) return NULL;
    arr->v = NULL;
    arr->n = 0;
    return arr;
}

bool arreglo_agregar(arreglo_t *a, void *e){
    void *aux = realloc(a->v, (a->n + 1) * sizeof(a->v));
    if (!aux)
        return false;
    a->v = aux;
    a->v[a->n++] = e;
    return true;
}

void arreglo_destruir(arreglo_t *arr, void (*eliminar)(void *)){
    for(size_t i = 0; i < arr->n ; i++)
        eliminar(arr->v[i]);
    free(arr->v);
    free(arr);
}