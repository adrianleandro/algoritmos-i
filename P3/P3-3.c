#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//punto 3
char *leer_cadena(FILE *f){
    size_t n;
    if(fread(&n, sizeof(size_t), 1, f) == 0) return "";
    char *cadena = malloc((n+1) * sizeof(char));
    if(!cadena) return NULL;
    for (size_t i = 0; i < n; i++) {
        fread(cadena+i, sizeof(char),1, f);
    }
    return cadena;
}

void destruir_cadena(char *cadena){
    free(cadena);
}

//main de pruebas
#define ARGUMENTOS_NECESARIOS 3
int main(int argc, char *argv[]){
    if(argc != ARGUMENTOS_NECESARIOS){
        fprintf(stderr, "Uso: %s [archivo] [numero de cadenas a imprimir]\n", argv[0]);
        return 1;
    }
    FILE *fi = fopen(argv[1], "rb");
    if(!fi){
        fprintf(stderr, "Error abriendo el archivo %s\n", argv[1]);
        return 1;
    }

    for(size_t i = 0; i < atoi(argv[2]); i++){
        char *cadena = leer_cadena(fi);
        if(!cadena){ // Error de memoria
            fprintf(stderr, "Error leyendo la cadena número %zd.\n", i);
            fclose(fi);
            return 1;
        }
        if(!strcmp(cadena, "")){ // leer_cadena me devuelve la cadena vacia
            fprintf(stderr, "Error: El archivo tiene %zd cadenas.\n", i);
            fclose(fi);
            return 1;
        }
        fputs(cadena, stdout);
        fputc('\n', stdout);
        destruir_cadena(cadena);
    }
    fclose(fi);
    return 0;
}
