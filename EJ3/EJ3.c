#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdbool.h>

//Constantes del EJ1
#define PI 3.1415926535897932

#define ANCHO 640
#define ALTO 480
#define FOV 90
//-------------------

//Constantes del EJ2
#define INFINITO FLT_MAX
//-------------------

// Definición de estructuras
typedef struct {
    float x, y, z;
} vector_t;

typedef struct {
    float r, g, b;
} color_t;

typedef struct {
    vector_t centro;
    float radio;
    color_t color;
    float ka, kd;
} esfera_t;

typedef struct {
    vector_t posicion; // Si es_puntual es una coordenada, si no una dirección
    color_t color;
    bool es_puntual;
} luz_t;

typedef struct {
    void **v;
    size_t n;
} arreglo_t;

// Funciones
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

color_t color_sumar(color_t c, color_t m, float p){
    color_t suma;
    suma.r = c.r + p * m.r;
    suma.g = c.g + p * m.g;
    suma.b = c.b + p * m.b;
    return suma;
}

color_t color_absorber(color_t b, color_t c){
    color_t absorcion;
    absorcion.r = b.r * c.r;
    absorcion.g = b.g * c.g;
    absorcion.b = b.b * c.b;
    return absorcion;
}

void color_imprimir(color_t c){
    if(c.r < 0) c.r = 0;
    if(c.g < 0) c.g = 0;
    if(c.b < 0) c.b = 0;
    if(c.r >= 1) c.r = 1;
    if(c.g >= 1) c.g = 1;
    if(c.b >= 1) c.b = 1;
    printf("%d %d %d", (int)(255 * c.r),(int)(255 * c.g),(int)(255 * c.b));
}

esfera_t *esfera_crear(vector_t centro, float radio, color_t color, float ka, float kd){
    esfera_t *esfera = malloc(sizeof(esfera_t));
    if(!esfera)
        return NULL;
    esfera->centro = centro;
    esfera->radio = radio;
    esfera->color = color;
    esfera->ka = ka;
    esfera->kd = kd;
    return esfera;
}

void esfera_destruir(esfera_t *esfera){
    free(esfera);
}

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual){
    luz_t *luz = malloc(sizeof(luz_t));
    if(!luz)
        return NULL;
    luz->posicion = posicion;
    luz->color = color;
    luz->es_puntual = es_puntual;
    return luz;
}

void luz_destruir(luz_t *luz){
    free(luz);
}

bool arreglo_agregar(arreglo_t *a, void *e){
    void *aux = realloc(a->v, (a->n + 1) * sizeof(a->v));
    if(!aux)
        return false;
    a->v = aux;
    a->v[a->n++] = e;
    return true;
}

float esfera_distancia(const esfera_t *esfera, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    vector_t origen_centro = vector_resta(esfera->centro,o); // origen_centro: C - O
    float beta = vector_producto_interno(origen_centro,d); // beta: (C - O) * d
    float delta = beta * beta - vector_producto_interno(origen_centro, origen_centro) + esfera->radio * esfera->radio; // delta: beta**2 - (C-O).(C-O) + R**2
    if(delta < 0)
        return INFINITO;
    float sqrt_delta = sqrt(delta);
    float t1 = beta + sqrt_delta;
    float t2 = beta - sqrt_delta;
    if(t1 < 0 && t2 < 0)
        return INFINITO;
    float t;
    if(t1 <= t2)
        t = t1;
    else
        t = t2;
    *punto = vector_interpolar_recta(o,d,t);
    *normal = vector_normalizar(vector_resta(*punto, esfera->centro));
    return t;
}

color_t computar_intensidad(const arreglo_t *esferas, const arreglo_t *luces, color_t ambiente, vector_t o, vector_t d){
    color_t intensidad = {0,0,0};
    float t = INFINITO;
    vector_t normal, punto, min_punto, min_normal;
    size_t minima;
    for(size_t i = 0; i < esferas->n; i++){
        esfera_t *esfera = esferas->v[i];
        float dist = esfera_distancia(esfera, o, d, &punto, &normal);
        if(dist < INFINITO && dist < t){
            t = dist;
            minima = i;
            min_punto = punto;
            min_normal = normal;
        }
    }
    if(t < INFINITO) {
        esfera_t *esfera = esferas->v[minima];
        intensidad = color_sumar(intensidad, ambiente, esfera->ka); // C = Ia * ka
        for (size_t j = 0; j < luces->n; j++) {
            char ds = 1;
            luz_t *luz = luces->v[j];
            vector_t direccion_luz = luz->posicion;
            if (luz->es_puntual) //si es puntual
                direccion_luz = vector_normalizar(vector_resta(luz->posicion, min_punto));
            for (size_t k = 0; k < esferas->n; k++) { // interseca con alguna esfera?
                if (minima == k) continue;
                if (esfera_distancia(esferas->v[k], min_punto, direccion_luz, &punto, &normal) < INFINITO)
                    ds = 0;
            }
            float ln = vector_producto_interno(direccion_luz, min_normal);
            if (!ds || ln < 0) continue;
            intensidad = color_sumar(intensidad, luz->color, ds * esfera->kd * ln);
        }
        intensidad = color_absorber(intensidad, esfera->color);
    }
    return intensidad;
}

int main(void) {
    arreglo_t luces = {NULL, 0};
    arreglo_t esferas = {NULL, 0};

    color_t ambiente = {.05, .05, .05};

    assert(arreglo_agregar(&luces, luz_crear(vector_normalizar((vector_t){0, 1, 0}), (color_t){.2, .2, .2}, false)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){-2, 10, 2.5}, (color_t){.9, 0, 0}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){2, 10, 2.5}, (color_t){0, .9, 0}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){0, 10, 5}, (color_t){0, 0, .9}, true)));

    assert(luces.n == 4);
    for(size_t i = 0; i < luces.n; i++)
        assert(luces.v[i] != NULL);

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 1, 2.4}, .3, (color_t){1, 1, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, -.4, 3}, 1, (color_t){1, 1, 1}, 1, 1)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, -.6, 3}, .3, (color_t){1, 0, 0}, 1, .8)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1.73, -.6, 2}, .3, (color_t){1, 1, 0}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, -.6, 1.26}, .3, (color_t){0, 1, 0}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, -.6, 1}, .3, (color_t){1, 1, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, -.6, 1.26}, .3, (color_t){0, 1, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1.73, -.6, 2}, .3, (color_t){0, 0, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, -.6, 3}, .3, (color_t){1, 0, 1}, 1, 1)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 0)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .16)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .5)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .66)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .83)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 1)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-3, 1.5, 4}, .3, (color_t){1, 1, 1}, 0, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, 1.5, 4}, .3, (color_t){1, 1, 1}, .16, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, 1.5, 4}, .3, (color_t){1, 1, 1}, .33, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 1.5, 4}, .3, (color_t){1, 1, 1}, .5, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, 1.5, 4}, .3, (color_t){1, 1, 1}, .66, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, 1.5, 4}, .3, (color_t){1, 1, 1}, .83, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){3, 1.5, 4}, .3, (color_t){1, 1, 1}, 1, 1)));

    assert(esferas.n == 23);
    for(size_t i = 0; i < esferas.n; i++)
        assert(esferas.v[i] != NULL);


    printf("P3\n");
    printf("%d %d\n", ANCHO, ALTO);
    printf("255\n");

    float vz = ANCHO / 2 / tan(FOV/ 2 * PI / 180);

    for(int vy = ALTO / 2; vy > - ALTO / 2; vy--)
        for(int vx = - ANCHO / 2; vx < ANCHO / 2; vx++) {
            color_imprimir(computar_intensidad(&esferas, &luces, ambiente, (vector_t){0, 0, 0}, vector_normalizar((vector_t){vx, vy, vz})));
            putchar('\n');
        }


    for(size_t i = 0; i < esferas.n; i++)
        esfera_destruir(esferas.v[i]);
    free(esferas.v);

    for(size_t i = 0; i < luces.n; i++)
        luz_destruir(luces.v[i]);
    free(luces.v);

    return 0;
}