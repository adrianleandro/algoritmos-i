#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct {
    float x, y, z;
} vector_t;

float vector_producto_interno(vector_t a, vector_t b);
float vector_norma(vector_t a);
vector_t vector_resta(vector_t a, vector_t b);
vector_t vector_interpolar_recta(vector_t o, vector_t d, float t);
vector_t vector_normalizar(vector_t a);

#endif