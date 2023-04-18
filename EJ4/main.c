#define MAX_ITER 80
#define PI 3.1415926535897932
#define FOV 90

#include "archivo.h"
#include "vector.h"
#include "imagen.h"
#include "color.h"

#include <stdlib.h>
#include <string.h>

bool tiene_extension(const char archivo[], const char extension[]){
    size_t i;
    size_t pos_sep_ext = 0; //Posicion del separador de extensión
    size_t long_archivo = strlen(archivo);
    size_t long_ext = strlen(extension);
    for(i = 0; i < long_archivo; i++){
        if(archivo[i] == extension[0])
            pos_sep_ext = i; //Posicion del ultimo separador de extension en el nombre del archivo
    }
    if(!pos_sep_ext) return false; //No tiene extension
    if(long_archivo - pos_sep_ext != long_ext) return false; // La longitud de la extension del archivo es distinta a la de la extension
    for(size_t j = 1; j < long_ext ; j++){
        if(archivo[pos_sep_ext+j] != extension[j])
            return false;
    }
    return true;
}

color_t computar_intensidad(color_t ambiente, vector_t o, vector_t d) {
    vector_t c = (vector_t){(d.x / d.z) * 1.5 - .5, (d.y / d.z) * 1.5 + .1, 0};
    vector_t z = {0, 0, 0};
    int n = 0;
    while(vector_norma(z) <= 2 && n < MAX_ITER) {
        z = (vector_t){z.x * z.x - z.y * z.y, z.x * z.y + z.x * z.y, 0};
        z = vector_interpolar_recta(z, c, 1);
        n++;
    }

    if(n == MAX_ITER)
        return ambiente;

    float h = (n + 1 - log(log2(vector_norma(z)))) / MAX_ITER * 360;
    float x = (1 - fabs(fmodf(h / 60.0, 2) - 1));
    float r = 0, g = 0, b = 0;

    if(h < 60)
        r = 1, g = x;
    else if(h < 120)
        g = 1, r = x;
    else if(h < 180)
        g = 1, b = x;
    else if(h < 240)
        b = 1, g = x;
    else if(h < 300)
        b = 1, r = x;
    else
        r = 1, b = x;

    return (color_t){r, g, b};
}

int main(int argc, char *argv[]) {
    //Validaciones
    if (argc != 4) {
        fprintf(stderr, "Uso: %s [ancho] [alto] [nombrearchivo]\n - El archivo debe ser de extensión .ppm o .bmp\n - El ancho y el alto deben ser números enteros y positivos.\n",
                argv[0]);
        return 1;
    }
    if (atoi(argv[1]) <= 0) {
        fprintf(stderr, "Error: el ancho ingresado no es un número válido (debe ser un número entero positivo)\n");
        return 1;
    }
    if (atoi(argv[2]) <= 0) {
        fprintf(stderr, "Error: el alto ingresado no es un número válido (debe ser un número entero positivo)\n");
        return 1;
    }
    if (!tiene_extension(argv[3], ".bmp") && !tiene_extension(argv[3], ".ppm")) {
        fprintf(stderr, "Error: el formato de archivo ingresado no es válido (debe ser .bmp o .ppm)\n");
        return 1;
    }

    // Datos válidos
    imagen_t *imagen = imagen_crear(atoi(argv[1]), atoi(argv[2]));
    if (!imagen) {
        fprintf(stderr, "Error creando la imagen\n");
        return 1;
    }
    printf("Generando la imagen %s ...\n", argv[3]);

    color_t ambiente = {.05, .05, .05};
    vector_t origen = {0, 0, 0};

    size_t ancho, alto;
    imagen_dimensiones(imagen, &ancho, &alto);

    float z = ancho / 2 / tan(FOV/ 2 * PI / 180);

    for(int y = (int)alto / 2; y > (int)-alto / 2; y--) {
        size_t vy = y+alto/2 - 1;
        for (int x = (int)-ancho / 2; x < (int)ancho / 2; x++) {
            size_t vx = x+ancho/2;
            imagen_set_pixel(imagen, vx, vy,computar_intensidad(ambiente, origen, vector_normalizar((vector_t) {x, y, z})));
        }
    }

    //caso archivo ppm
    if(tiene_extension(argv[3], ".ppm")) {
        FILE *imagenppm = fopen(argv[3], "wt");
        if (!imagenppm) {
            fprintf(stderr, "Error abriendo el archivo a escribir. Abortando programa...\n");
            imagen_destruir(imagen);
            return 1;
        }
        escribir_PPM(imagen, imagenppm);
        fclose(imagenppm);

        imagen_destruir(imagen);
        printf("Se generó la imagen %s con éxito.\n", argv[3]);
        return 0;
    }

    //caso archivo bmp
    FILE *imagenbmp = fopen(argv[3], "wb");
    if (!imagenbmp) {
        fprintf(stderr, "Error abriendo el archivo a escribir. Abortando programa...\n");
        imagen_destruir(imagen);
        return 1;
    }
    escribir_BMP(imagen, imagenbmp);
    fclose(imagenbmp);

    imagen_destruir(imagen);
    printf("Se generó la imagen %s con éxito.\n", argv[3]);
    return 0;
}