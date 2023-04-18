#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//punto 1
#define MAX_STRING 128
#define MAX_CANDIDATOS 50 //Puede haber hasta 50 candidatos

typedef struct{
    char candidatos[MAX_CANDIDATOS][MAX_STRING]; //Arreglo de cadenas con los nombres de los candidatos. Cada candidato tendra un nombre de hasta MAX_STRING de largo
    size_t votos[MAX_CANDIDATOS]; //Arreglo con el numero de votos de cada candidato.
    size_t n; //Cantidad total de candidatos. candidatos == NULL y votos == NULL si y solo si n == 0
} votacion_t;

const char *votacion_ganador(const votacion_t *v){
    size_t ganador = 0;
    for(size_t i = 0; i < v->n; i++){
        if(v->votos[i] >= v->votos[ganador])
            ganador = i;
    }
    return v->candidatos[ganador];
}

bool votacion_votar(votacion_t *v, const char *nombre){
    for(size_t i = 0; i < v->n; i++){ // Existe el candidato
        if(!strcmp(v->candidatos[i], nombre)) {
            (v->votos[i])++;
            return true;
        }
    }
    //No existe el candidato
    if((v->n)+1 >= MAX_CANDIDATOS) return false; //Si al querer agregar un candidato supero MAX_CANDIDATOS devuelvo false
    strcpy(v->candidatos[v->n], nombre);
    v->votos[v->n] = 1;
    v->n++;
    return true;
}

//punto 2
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
    //prueba punto 1
    puts("-------PUNTO 1--------");
    votacion_t votacion ={
            {"Tinelli", "Chiqui Tapia"},
            {37, 36},
            2
    };
    printf("El ganador seria %s\nPero faltan 5 votos\n", votacion_ganador(&votacion));
    votacion_votar(&votacion, "Tinelli");
    votacion_votar(&votacion, "Chiqui Tapia");
    votacion_votar(&votacion, "Chiqui Tapia");
    votacion_votar(&votacion, "Chiqui Tapia");
    votacion_votar(&votacion, "La tumba de Grondona");
    printf("5 votos nuevos emitidos.\nEl ganador es %s\n", votacion_ganador(&votacion));

    //prueba punto 2 y punto 3
    puts("-----PUNTOS 2 Y 3-----");
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