#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "imagen.h"

#include <stdio.h>
#include <stdint.h>

#define MAX_INTENSIDAD_PPM 255
#define MAX_INTENSIDAD_BMP 255

void escribir_PPM(const imagen_t *imagen, FILE *f);
void escribir_int16_little_endian(FILE *f, int16_t v);
void escribir_int32_little_endian(FILE *f, int32_t v);
void escribir_BMP(const imagen_t *imagen, FILE *f);

#endif