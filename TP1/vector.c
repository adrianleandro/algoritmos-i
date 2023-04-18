#include "vector.h"

vector_t vector_crear(float x, float y, float z){
    vector_t vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;
    return vector;
}

float vector_producto_interno(vector_t a, vector_t b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vector_norma(vector_t a){
    return sqrt(vector_producto_interno(a,a));
}

vector_t vector_por_escalar(vector_t a, float n){
    vector_t vector;
    vector.x = n * a.x;
    vector.y = n * a.y;
    vector.z = n * a.z;
    return vector;
}

vector_t vector_resta(vector_t a, vector_t b){
    vector_t resta;
    resta.x = a.x - b.x;
    resta.y = a.y - b.y;
    resta.z = a.z - b.z;
    return resta;
}

vector_t producto_vectorial(vector_t a, vector_t b){
    vector_t prod;
    prod.x = a.y * b.z - b.y * a.z;
    prod.y = b.x * a.z - a.x * b.z;
    prod.z = a.x * b.y - b.x * a.y;
    return prod;
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
