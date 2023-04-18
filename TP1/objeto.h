#ifndef OBJETO_H
#define OBJETO_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#include "color.h"
#include "arreglo.h"
#include "vector.h"

#define INFINITO FLT_MAX
#define EPSILON 10e-6

typedef enum forma {TRIANGULO, MALLA, ESFERA, PLANO} forma_t;

typedef struct {
    forma_t forma;
    void *geometria; //Dependiendo de la forma, será un puntero a una estructura que contenga la geometria de esta.
    color_t color;
    float ka, kd, ks, kr;
} objeto_t;

typedef struct {
    vector_t centro;
    float radio;
} esfera_t;

typedef struct{
    vector_t normal, p0, p1, p2;
} triangulo_t;

typedef struct{
    vector_t normal, p0;
} plano_t;

typedef struct{
    objeto_t **malla;
    size_t n;
} malla_t;

typedef struct {
    vector_t posicion; // Si es_puntual es una coordenada, si no una dirección
    color_t color;
    bool es_puntual;
} luz_t;

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual);
void luz_destruir(void *luz);

objeto_t *triangulo_crear(vector_t normal, vector_t p0, vector_t p1, vector_t p2, color_t color, float ka, float kd, float ks, float kr);
objeto_t *esfera_crear(vector_t centro, float radio, color_t color, float ka, float kd, float ks, float kr);
objeto_t *plano_crear(vector_t normal, vector_t p0, color_t color, float ka, float kd, float ks, float kr);
objeto_t *malla_crear(const char *ruta, vector_t posicion, float escala, color_t color, float ka, float kd, float ks, float kr);

void objeto_destruir(void *obj);

float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal);

#endif