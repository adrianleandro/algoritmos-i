#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3
#define M 2

#define MAX 256

//punto 1
bool tiene_extension(const char archivo[], const char extension[]){
    size_t i;
    for(i = 0; archivo[i] != '.' && archivo[i]; i++){}
    if(!archivo[i]) return false; //No tiene extension
    for (size_t j = 1; extension[j] ; j++) {
        if(archivo[i+j] != extension[j]) return false;
    }
    return true;
}

//punto 2
bool es_nonegativa(size_t n, size_t m, float matriz[n][m]){
    for(size_t y = 0; y < n; y++){
        for (size_t x = 0; x < m ; x++) {
            if(matriz[y][x] < 0) return false;
        }
    }
    return true;
}

//punto 3
float grados_a_decimal(int grados, int minutos, int segundos){
    while(segundos >= 60){
        segundos = segundos / 60 + segundos % 60;
        minutos++;
    }
    while(minutos >= 60){
        minutos = minutos / 60.0 + minutos % 60;
        grados++;
    }
    return grados + minutos/60.0 + (segundos/60.0)/60;
}


int main(void){
    //prueba punto 1
    char archivo[] = "listado.txt";
    char extension[] = ".txt";
    printf("%s tiene la extension\n", tiene_extension(archivo,extension) ? "Si" : "No");

    putchar('\n');

    //prueba punto 2
    float matriz[N][M] = {
            {7.8,4.5},
            {1.2,0.5},
            {7.2,1.7}
    };
    printf("La matriz %s es no-negativa\n", es_nonegativa(N,M,matriz) ? "si" : "no");

    putchar('\n');

    //punto 3
    char buffer[MAX];
    printf("Ingrese los grados:\n");
    int grados = atoi(fgets(buffer, MAX, stdin));
    printf("Ingrese los minutos:\n");
    int minutos = atoi(fgets(buffer, MAX, stdin));
    printf("Ingrese los segundos:\n");
    int segundos = atoi(fgets(buffer, MAX, stdin));

    //      No valido si son mayores de 60 porque en ese caso la funcion convierte minutos a grados y segundos a minutos.
    if(minutos < 0 || segundos < 0){
        printf("Los minutos y los segundos no pueden ser negativos\n");
        return 1;
    }

    if(grados < 0 || grados > 360){
        printf("Los grados ingresados no son validos. deben estar comprendidos entre 0 y 360\n");
        return 1;
    } else {
        printf("%.4f\n", grados_a_decimal(grados, minutos, segundos));
    }

    return 0;
}