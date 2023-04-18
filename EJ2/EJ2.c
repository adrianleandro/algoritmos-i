#include <stdio.h>
#include <math.h>
#include <float.h>

//Constantes del EJ1
#define PI 3.1415926535897932

#define ANCHO 640
#define ALTO 480
#define FOV 90
//-------------------

#define INFINITO FLT_MAX

#define IA  5
#define II  255

const float origen[] = {0.0,0.0,0.0};

const float luz[3] = {0.2873478855663454, 0.9578262852211513, 0};

const float centros[][3] = {
        {-.4, .75, 1.55},
        {-.55, -.4, 2},
        {3, 1.5, 4},
        {3, 1.5, 5},
        {3, -1.5, 4},
        {3, -1.5, 5},
};

const float radios[] = {
        .3,
        .85,
        .4,
        .4,
        .4,
        .4,
};

float producto_interno(const float a[3], const float b[3]){
    float producto = 0;
    for(size_t i = 0; i < 3; i++)
        producto += a[i] * b[i];
    return producto;
}

float norma(const float a[3]){
    return sqrt(producto_interno(a,a));
}

void normalizar(float a[3]){
    float modulo = norma(a);
    for(size_t i = 0; i < 3; i++)
        a[i] /= modulo;
}

void resta(float r[3], const float a[3], const float b[3]){
    for(size_t i = 0; i < 3; i++)
        r[i] = a[i] - b[i];
}

void interpolar_recta(float p[3], const float o[3], const float d[3], float t){
    for(size_t i = 0; i < 3; i++)
        p[i] = o[i] + t * d[i];
}

float distancia_esfera(const float c[3], float r, const float o[3], const float d[3]){
    float origen_centro[3];
    resta(origen_centro, c,o); // origen_centro: C - O
    float beta = producto_interno(origen_centro,d); // beta: (C - O) * d
    float delta = beta * beta - producto_interno(origen_centro, origen_centro) + r * r; // delta: beta**2 - (C-O).(C-O) + R**2
    if(delta < 0)
        return INFINITO;
    float sqrt_delta = sqrt(delta);
    float t1 = beta + sqrt_delta;
    float t2 = beta - sqrt_delta;
    if(t1 < 0 && t2 < 0) {
        return INFINITO;
    } else {
        if(t1 <= t2)
            return t1;
        else
            return t2;
    }
}

void normal_esfera(float normal[3], const float c[3], float r, const float p[3]){
    resta(normal,p,c);
    normalizar(normal);
}

int computar_intensidad(const float cs[][3], const float rs[], size_t n_esferas, const float luz[3], const float o[3], const float d[3]){
    float normal[3], punto[3];
    float t;
    for(size_t i = 0; i < n_esferas; i++) {
        t = distancia_esfera(cs[i], rs[i], o, d);
        if(t < INFINITO){
            interpolar_recta(punto,o,d,t);
            char ds = 1;
            for(size_t j = 0; j < n_esferas; j++) {
                if(i == j) continue;
                t = distancia_esfera(cs[j], rs[j], punto, luz);
                if (t < INFINITO) {
                    ds = 0;
                }
            }
            if(ds) normal_esfera(normal, cs[i], rs[i], punto);
            int iluminacion = IA + ds * (II * producto_interno(luz, normal));
            if(iluminacion <= IA) return IA;
            if(iluminacion >= II) return II;
            return iluminacion;
        }
    }
    return 0;
}

int main(void){
    printf("P2\n%d %d\n%d\n", ANCHO, ALTO, II); //Encabezado
    float z = ANCHO/(2*tan((FOV/2) * (PI/180)));
    float direccion[3];
    size_t cant_esferas = sizeof(radios)/sizeof(radios[0]);
    for(int y = ALTO/2; y > -ALTO/2; y--) {
        for (int x = -ANCHO/2; x < ANCHO/2; x++) {
            direccion[0] = (float)x;
            direccion[1] = (float)y;
            direccion[2] = z;
            normalizar(direccion);
            printf("%d", computar_intensidad(centros,radios,cant_esferas,luz,origen,direccion));
            putchar(' ');
        }
        putchar('\n');
    }
    return 0;
}
