#include "lista.h"

#include <stdlib.h>

struct nodo {
    struct nodo *sig;
    void *dato;
};

struct lista {
    struct nodo *prim;
};

static struct nodo *crear_nodo(void *dato, struct nodo *sig) {
    struct nodo *n = malloc(sizeof(struct nodo));
    if(n == NULL) return NULL;

    n->dato = dato;
    n->sig = sig;

    return n;
}


lista_t *lista_crear(void) {
    return calloc(1, sizeof(lista_t));
/*
    lista_t *l = malloc(sizeof(lista_t));
    if(l == NULL) return NULL;

    l->prim = NULL;

    return l;*/
}

void lista_destruir(lista_t *l, void (*destruir)(void *)) {
    struct nodo *actual = l->prim;
    while(actual != NULL) {
        struct nodo *siguiente = actual->sig;

        if(destruir != NULL)
            destruir(actual->dato);
        free(actual);

        actual = siguiente;
    }

    free(l);
}

bool lista_es_vacia(const lista_t *l) {
    return l->prim == NULL;
}


bool lista_agregar_al_principio(lista_t *l, void *dato) {
    struct nodo *n = crear_nodo(dato, l->prim);
    if(n == NULL) return false;

    l->prim = n;

    return true;
}

bool lista_agregar_al_final(lista_t *l, void *dato) {
    struct nodo *n = crear_nodo(dato, NULL);
    if(n == NULL) return false;

    if(l->prim == NULL) {
        l->prim = n;
        return true;
    }

    struct nodo *actual = l->prim;
    while(actual->sig != NULL) {

        actual = actual->sig;
    }

    actual->sig = n;

    return true;
}

void *lista_borrar(lista_t *l, const void *dato, int (*cmp)(const void *, const void *)) {
    struct nodo *anterior = NULL;
    struct nodo *actual = l->prim;
    while(actual != NULL && cmp(actual->dato, dato) != 0) {
        anterior = actual;
        actual = actual->sig;
    }
    if(actual == NULL)
        return NULL;

    if(anterior == NULL)
        l->prim = actual->sig;
    else
        anterior->sig = actual->sig;

    void *aux = actual->dato;
    free(actual);
    return aux;
}

void lista_recorrer(const lista_t *l, bool (*visitar)(void *dato, void *extra), void *extra) {
    struct nodo *actual = l->prim;
    while(actual != NULL) {
        if(! visitar(actual->dato, extra))
            return;

        actual = actual->sig;
    }
}


static void nodo_recorrer_recursiva(struct nodo *n, bool (*visitar)(void *dato, void *extra), void *extra) {
    if(n == NULL) return;
    nodo_recorrer_recursiva(n->sig, visitar, extra);
    visitar(n, extra);
}

void lista_recorrer_recursiva(const lista_t *l, bool (*visitar)(void *dato, void *extra), void *extra) {
    nodo_recorrer_recursiva(l->prim, visitar, extra);
}

#ifdef TEST_LISTA

#include <stdio.h>
#include <assert.h>

int main(void) {
    lista_t *l;

    assert((l = lista_crear()) != NULL);
    assert(lista_es_vacia(l));
    assert(lista_agregar_al_principio(l, "hola"));
    assert(lista_agregar_al_principio(l, "que"));
    assert(lista_agregar_al_principio(l, "tal"));

    lista_destruir(l, NULL);
    return 0;
}

#endif
