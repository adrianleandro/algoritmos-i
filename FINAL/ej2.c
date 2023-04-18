#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // HACER: implementar el programa.
    //
    // Para probarlo:
    //
    // ./ej2 50 audio.bin audio2.bin
    //
    // debe generar el archivo `audio2.bin`, del mismo tamaño en bytes que
    // `audio.bin`, con las muestras de audio al 50% de magnitud.
    //
    // Luego:
    //
    // ./ej2 200 audio2.bin audio3.bin
    //
    // debe generar el archivo `audio3.bin`, con las muestras al 200% de
    // magnitud, es decir que `audio.bin` y `audio3.bin` deben ser iguales al
    // finalizar.

    if(argc != 4){
        fprintf(stderr, "Error. Uso: %s Valor Entrada Salida\n", argv[0]);
        return 1;
    }

    float v = atoi(argv[1]);
    if(v < 0){
        fprintf(stderr, "Error. V debe ser un numero positivo\n");
        return 1;
    }

    FILE *fi = fopen(argv[2] ,"rb");
    if(!fi){
        fprintf(stderr, "Error abriendo el archivo %s\n", argv[2]);
        fclose(fi);
        return 1;
    }

    FILE *fo = fopen(argv[3], "wb");
    if(!fo){
        fclose(fi);
        fprintf(stderr, "Error abriendo el archivo %s\n", argv[3]);
        return 1;
    }

    float buff_flot;
    while (fread(&buff_flot, sizeof(float), 1, fi)){
        buff_flot *= (v/100);
        fwrite(&buff_flot, sizeof(float), 1, fo);
    }
    fclose(fi);
    fclose(fo);
    return 0;
}
