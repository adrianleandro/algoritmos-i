#include "vector.h"

float vector_producto_interno(vector_t a, vector_t b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vector_norma(vector_t a){
    return sqrt(vector_producto_interno(a,a));
}

vector_t vector_resta(vector_t a, vector_t b){
    vector_t resta;
    resta.x = a.x - b.x;
    resta.y = a.y - b.y;
    resta.z = a.z - b.z;
    return resta;
}

vector_t vector_interpolar_recta(vector_t o, vector_t d, float t){
    vector_t punto;
    punto.x = o.x + t * d.x;
    punto.y = o.y + t * d.y;
    punto.z = o.z + t * d.z;
    return punto;
}

vector_t vector_normalizar(vector_t a){
    vector_t normalizado;
    float norma = vector_norma(a);
    normalizado.x = a.x / norma;
    normalizado.y = a.y / norma;
    normalizado.z = a.z / norma;
    return normalizado;
}
