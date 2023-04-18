#define PI 3.1415926535897932
#define FOV 90

#include "archivo.h"
#include "vector.h"
#include "imagen.h"
#include "color.h"
#include "arreglo.h"
#include "objeto.h"

#include <assert.h>

color_t computar_intensidad(int profundidad, const arreglo_t *objetos, const arreglo_t *luces, color_t ambiente, vector_t o, vector_t d){
    color_t intensidad = {0,0,0};
    float t = INFINITO;
    vector_t normal, punto, min_punto, min_normal;
    size_t minimo;
    for(size_t i = 0; i < objetos->n; i++){
        objeto_t *obj = objetos->v[i];
        float dist = objeto_distancia(obj, o, d, &punto, &normal);
        if(dist < INFINITO && dist < t){
            t = dist;
            minimo = i;
            min_punto = punto;
            min_normal = normal;
        }
    }
    if(t < INFINITO) {
        objeto_t *obj = objetos->v[minimo];
        intensidad = color_sumar(intensidad, ambiente, obj->ka); // C = Ia * ka
        vector_t vector_r = vector_resta(d, vector_por_escalar(min_normal, 2 * vector_producto_interno(d,min_normal)));
        for (size_t j = 0; j < luces->n; j++) {
            char ds = 1;
            luz_t *luz = luces->v[j];
            vector_t direccion_luz = luz->posicion;
            if (luz->es_puntual) //si es puntual
                direccion_luz = vector_normalizar(vector_resta(luz->posicion, min_punto));
            for (size_t k = 0; k < objetos->n; k++) { // interseca con algun objeto?
                if (minimo == k) continue;
                if (objeto_distancia(objetos->v[k], min_punto, direccion_luz, &punto, &normal) < INFINITO)
                    ds = 0;
            }
            float ln = vector_producto_interno(direccion_luz, min_normal);
            float lr = vector_producto_interno(direccion_luz, vector_r);
            if (!ds || ln < 0) ln = 0;
            if (!ds || lr < 0) lr = 0;
            intensidad = color_sumar(intensidad, luz->color, ds * obj->kd * ln);
            intensidad = color_sumar(intensidad, luz->color, ds * obj->ks * pow(lr,ALPHA));
        }
        intensidad = color_absorber(intensidad, obj->color);
        if(obj->kr > 0 && profundidad > 0){
            vector_t p_prima = vector_interpolar_recta(min_punto, vector_r, 10*EPSILON);
            intensidad = color_sumar(intensidad, computar_intensidad(profundidad - 1, objetos, luces, ambiente, p_prima, vector_r), obj->kr);
        }
    }
    return color_limitar(intensidad);
}

#define CORRIMIENTO_X_BALDOSA 1.2
#define CORRIMIENTO_Z_BALDOSA 2.3
#define CORRIMIENTO_PARES 1.5
#define COLOR_BALDOSA (color_t){0.5,0,0}
#define KA_PISO 0.8
#define KD_PISO 0.6
#define KS_PISO 1
#define KR_PISO 1

int main(int argc, char *argv[]){
    //Validaciones
    if (argc != 5) {
        fprintf(stderr, "Uso: %s [ancho] [alto] [profundidad] [nombrearchivo]\n - El archivo debe ser de extensión .ppm o .bmp\n - La profundidad debe ser 0 o mayor.\n - El ancho y el alto deben ser números enteros y positivos.\n",
                argv[0]);
        return 1;
    }
    size_t ancho = atoi(argv[1]);
    if (ancho <= 0) {
        fprintf(stderr, "Error: el ancho ingresado no es un número válido (debe ser un número entero positivo)\n");
        return 1;
    }
    size_t alto = atoi(argv[2]);
    if (alto <= 0) {
        fprintf(stderr, "Error: el alto ingresado no es un número válido (debe ser un número entero positivo)\n");
        return 1;
    }
    size_t profundidad = atoi(argv[3]);
    if (profundidad < 0) {
        fprintf(stderr, "Error: la profundidad ingresada no es un número válido (debe ser un número entero positivo o 0)\n");
        return 1;
    }
    if (!tiene_extension(argv[4], ".bmp") && !tiene_extension(argv[4], ".ppm")) {
        fprintf(stderr, "Error: el formato de archivo ingresado no es válido (debe ser .bmp o .ppm)\n");
        return 1;
    }

    //Creado de escena

    arreglo_t *luces = arreglo_crear();
    arreglo_t *objetos = arreglo_crear();

    vector_t origen = {0,0,0};
    color_t ambiente = {0.05, 0.05, 0.05};

    assert(arreglo_agregar(luces, luz_crear(vector_normalizar((vector_t){0, 1, 2.4}), (color_t){1, 1, 1}, false)));
    assert(arreglo_agregar(luces, luz_crear(vector_normalizar((vector_t){1, 1, 1}), (color_t){1, 1, 1}, true)));
    assert(arreglo_agregar(luces, luz_crear(vector_normalizar((vector_t){0,24,95}), (color_t){1, 1, 1}, true)));


    assert(arreglo_agregar(objetos, esfera_crear((vector_t){0.3, -0.7, 2.4}, .3, (color_t){1, 1, 0}, 0.33, 1, 0.98, 0.8)));
    assert(arreglo_agregar(objetos, esfera_crear((vector_t){1.6, -0.5, 4}, .5, (color_t){0, 0, 1}, 1, 1, 0.7, 0.2)));
    assert(arreglo_agregar(objetos, esfera_crear((vector_t){-0.7, -0.7, 3}, .3, (color_t){0, 0, 1}, 0.6, 1, 0.6, 0.6)));

    assert(arreglo_agregar(objetos, esfera_crear((vector_t){-5, 0.5, 12}, 1.5, (color_t){0, 0, 1}, 0.7, 0.6, 0.3, 0.2)));
    assert(arreglo_agregar(objetos, esfera_crear((vector_t){-1, -0.4, 6}, 0.6, (color_t){1, 1, 0}, 0.9, 0.5, 0.2, 0.6)));
    assert(arreglo_agregar(objetos, esfera_crear((vector_t){7, 0.2, 10}, 1.2, (color_t){0, 0, 1}, 0.1, 0.3, 1, 0.5)));

    assert(arreglo_agregar(objetos, plano_crear(vector_normalizar((vector_t){0, 1, 0}), (vector_t){0, -1 - EPSILON, 2.4}, (color_t){.8, .8, .8}, KA_PISO, KD_PISO, KS_PISO, 0)));

    assert(arreglo_agregar(objetos, plano_crear(vector_normalizar((vector_t){1, 0, 0}), (vector_t){-10, -1, 0}, (color_t){0, 0, 0}, 0, 0, 0, 1))); // Espejo


    for(size_t j = 0; j <= 11; j++) {
        int j_par = j % 2;
        for (size_t i = 0; i <= 10; i++) {
            assert(arreglo_agregar(objetos, triangulo_crear(vector_normalizar((vector_t) {0, 1, 0}),
                                                            (vector_t) {-8 + CORRIMIENTO_X_BALDOSA * j, -1, 0 + CORRIMIENTO_Z_BALDOSA * i - j_par * CORRIMIENTO_PARES},
                                                            (vector_t) {-8 + CORRIMIENTO_X_BALDOSA * j, -1, 2 + CORRIMIENTO_Z_BALDOSA * i - j_par * CORRIMIENTO_PARES},
                                                            (vector_t) {-7 + CORRIMIENTO_X_BALDOSA * j, -1, 0 + CORRIMIENTO_Z_BALDOSA * i - j_par * CORRIMIENTO_PARES},
                                                            COLOR_BALDOSA, KA_PISO, KD_PISO, KS_PISO, KR_PISO)));
            assert(arreglo_agregar(objetos, triangulo_crear(vector_normalizar((vector_t) {0, 1, 0}),
                                                            (vector_t) {-7 + CORRIMIENTO_X_BALDOSA * j, -1 + EPSILON, 0 + CORRIMIENTO_Z_BALDOSA * i - j_par * CORRIMIENTO_PARES},
                                                            (vector_t) {-7 + CORRIMIENTO_X_BALDOSA * j, -1 + EPSILON, 2 + CORRIMIENTO_Z_BALDOSA * i - j_par * CORRIMIENTO_PARES},
                                                            (vector_t) {-8 + CORRIMIENTO_X_BALDOSA * j, -1 + EPSILON, 2 + CORRIMIENTO_Z_BALDOSA * i - j_par * CORRIMIENTO_PARES},
                                                            COLOR_BALDOSA, KA_PISO, KD_PISO, KS_PISO, KR_PISO)));
        }
    }

    assert(arreglo_agregar(objetos, malla_crear("src/gato.stl", (vector_t){175, -11.58, 80}, 1,(color_t){0.8, 0.8, 0.8}, 0.7, 0.8, 0.9, 1)));

    assert(arreglo_agregar(objetos, malla_crear("src/pajaro.stl", (vector_t){-5, 10, 20}, 0.035, (color_t){0.6, 0.2, 0}, 0, 0.6, 1, 0.5)));

    //Fin escena

    // Datos válidos
    imagen_t *imagen = imagen_crear(ancho, alto);
    if (!imagen) {
        fprintf(stderr, "Error creando la imagen\n");
        return 1;
    }
    printf("Generando la imagen %s ...\n", argv[4]);

    float z = ancho / (2 * tan(FOV/ 2 * PI / 180));

    for(int y = (int)alto / 2; y > (int)-alto / 2; y--) {
        size_t vy = y+alto/2 - 1;
        for (int x = (int)-ancho / 2; x < (int)ancho / 2; x++) {
            size_t vx = x+ancho/2;
            imagen_set_pixel(imagen, vx, vy,computar_intensidad(profundidad, objetos, luces, ambiente, origen, vector_normalizar((vector_t) {x, y, z})));
        }
    }

    //caso archivo ppm
    if(tiene_extension(argv[4], ".ppm")) {
        FILE *imagenppm = fopen(argv[4], "wt");
        if (!imagenppm) {
            fprintf(stderr, "Error abriendo el archivo a escribir. Abortando programa...\n");
            imagen_destruir(imagen);
            return 1;
        }
        escribir_PPM(imagen, imagenppm);
        fclose(imagenppm);

        imagen_destruir(imagen);
        printf("Se generó la imagen %s con éxito.\n", argv[4]);
        return 0;
    }

    //caso archivo bmp
    FILE *imagenbmp = fopen(argv[4], "wb");
    if (!imagenbmp) {
        fprintf(stderr, "Error abriendo el archivo a escribir. Abortando programa...\n");
        imagen_destruir(imagen);
        return 1;
    }
    escribir_BMP(imagen, imagenbmp);
    fclose(imagenbmp);

    imagen_destruir(imagen);
    printf("Se generó la imagen %s con éxito.\n", argv[4]);

    arreglo_destruir(luces, luz_destruir);
    arreglo_destruir(objetos, objeto_destruir);

    return 0;
}
