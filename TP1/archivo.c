#include "archivo.h"

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

void escribir_PPM(const imagen_t *imagen, FILE *f) {
    //Encabezado
    size_t dimensiones[2];
    imagen_dimensiones(imagen, dimensiones, dimensiones + 1);
    fprintf(f, "P3\n%zd %zd\n%d\n", dimensiones[0], dimensiones[1], MAX_INTENSIDAD_PPM);

    //Pixeles
    for(size_t y = dimensiones[1] - 1; y > 0; y--) {
        for (size_t x = 0; x < dimensiones[0]; x++) {
            color_t pixel = imagen_get_pixel(imagen, x, y);

            fprintf(f, "%d %d %d\n", (int)(MAX_INTENSIDAD_PPM * pixel.r), (int)(MAX_INTENSIDAD_PPM * pixel.g), (int)(MAX_INTENSIDAD_PPM * pixel.b));
        }
    }
}

#define MASK_BYTE 0xFF
void escribir_int16_little_endian(FILE *f, int16_t v){
    int8_t byte = v & MASK_BYTE;
    fwrite(&byte, sizeof(int8_t), 1, f);
    byte = (v >> 8); //ISO-C99 me garantiza ceros a la izquierda asi que no es necesario hacer el and con la mascara
    fwrite(&byte, sizeof(int8_t), 1, f);
}

#define MASK_INT16 0xFFFF
void escribir_int32_little_endian(FILE *f, int32_t v){
    escribir_int16_little_endian(f, v & MASK_INT16);
    escribir_int16_little_endian(f, (v >> 16)); //ISO-C99 me garantiza ceros a la izquierda asi que no es necesario hacer el and con la mascara
}

void escribir_BMP(const imagen_t *imagen, FILE *f){
    size_t dimensiones[2];
    imagen_dimensiones(imagen, dimensiones, dimensiones + 1);

    size_t scanline = dimensiones[0] * 3;
    scanline += scanline % 4;
    size_t tamanio = 54 + dimensiones[1] * scanline;

    //Encabezado de archivo
    char *encabezado = "BM";
    fwrite(encabezado, sizeof(char), 2, f);
    escribir_int32_little_endian(f, tamanio);
    escribir_int16_little_endian(f,0); //Reservado
    escribir_int16_little_endian(f,0); //Reservado
    escribir_int32_little_endian(f,54); //Offset

    //Encabezado de imagen
    escribir_int32_little_endian(f,40); //Tamaño
    escribir_int32_little_endian(f, dimensiones[0]); //Ancho
    escribir_int32_little_endian(f, dimensiones[1]); //Alto
    escribir_int16_little_endian(f,1); //Planos
    escribir_int16_little_endian(f,24); //Bits de color
    for(size_t i = 0; i < 6; i++)
        escribir_int32_little_endian(f,0); //Compresion, tamaño de imagen, resolucion x, resolucion y, tablas de color y colores importantes son todos int32 de valor 0.

    //Pixeles
    uint8_t ceros[] = {0,0,0};
    for(size_t y = 0; y < dimensiones[1]; y++) {
        size_t bytes = 0;
        for (int x = 0; x < dimensiones[0]; x++) {
            color_t pixel = imagen_get_pixel(imagen, x, y);
            uint8_t colores[3] = {MAX_INTENSIDAD_BMP * pixel.b, MAX_INTENSIDAD_BMP * pixel.g, MAX_INTENSIDAD_BMP * pixel.r};
            bytes += fwrite(colores, sizeof(uint8_t), 3, f);
        }
        fwrite(ceros, sizeof(uint8_t), bytes % 4, f);
    }
}