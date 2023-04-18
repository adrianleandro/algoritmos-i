#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

struct lista;
typedef struct lista lista_t;


lista_t *lista_crear();
void lista_destruir(lista_t *l, void (*destruir_dato)(void *));

bool lista_es_vacia(const lista_t *l);
bool lista_agregar_al_principio(lista_t *l, void *dato);
bool lista_agregar_al_final(lista_t *l, void *dato);
void *lista_borrar_primero(lista_t *l);
void *lista_borrar_ultimo(lista_t *l);

void *lista_buscar(const lista_t *l, const void *dato, int (*cmp)(const void *, const void *));
void *lista_borrar(lista_t *l, const void *dato, int (*cmp)(const void *, const void *));

void lista_recorrer(const lista_t *l, bool (*visitar)(void *dato, void *extra), void *extra);

#endif
