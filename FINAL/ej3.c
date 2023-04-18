#include <stdio.h>
#include <assert.h>

typedef struct nodo {
    int dato;
    struct nodo *izq;
    struct nodo *der;
} nodo_t;

int cantidad_hojas(nodo_t *n) {
    // HACER: implementar la funcion
    if(n == NULL) return 0;
    if(n->izq == NULL && n->der == NULL) return 1;
    if(n->izq == NULL) return cantidad_hojas(n->der);
    if(n->der == NULL) return cantidad_hojas(n->izq);
    return cantidad_hojas(n->izq) + cantidad_hojas(n->der);
}

int main(int argc, const char *argv[]) {
    // armamos "a mano" el arbol del ejemplo
    nodo_t a = {.dato = 'A'};
    nodo_t b = {.dato = 'B'};
    nodo_t c = {.dato = 'C'};
    nodo_t d = {.dato = 'D'};
    nodo_t e = {.dato = 'E'};
    nodo_t f = {.dato = 'F'};
    nodo_t g = {.dato = 'G'};
    nodo_t h = {.dato = 'H'};
    nodo_t i = {.dato = 'I'};
    b.der = &d;
    b.izq = &a;
    d.der = &e;
    d.izq = &c;
    f.der = &g;
    f.izq = &b;
    g.der = &i;
    i.izq = &h;

    assert(cantidad_hojas(&f) == 4);

    // OPCIONAL: escribir pruebas adicionales

    printf("%s: OK\n", __FILE__);
    return 0;
}
