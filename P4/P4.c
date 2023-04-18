#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//punto 1
typedef struct {
    struct nodo *prim;
} lista_ordenada_t;

struct nodo {
    struct nodo *sig;
    int dato;
};

typedef struct nodo nodo_t;

bool lista_ordenada_insertar(lista_ordenada_t *l, int dato){
    nodo_t *actual = l->prim;
    if(actual == NULL){ //Lista vacia
        nodo_t *nodo = malloc(sizeof(nodo_t));
        if(nodo == NULL) return false;
        nodo->dato = dato;
        nodo->sig = NULL;
        l->prim = nodo;
        return true;
    }
    while(actual->sig != NULL) {
        nodo_t *sig = actual->sig;
        if(actual->dato <= dato && dato <= sig->dato){ // Busco el lugar en el que va el nodo para cumplir el invariante de que la lista esté ordenada
            nodo_t *nodo = malloc(sizeof(nodo_t));
            if(nodo == NULL) return false;
            nodo->dato = dato;
            actual->sig = nodo;
            nodo->sig = sig;
            return true;
        }
        actual = actual->sig;
    }
    //Inserto al final, es mayor que todos los datos actuales
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL) return false;
    nodo->dato = dato;
    nodo->sig = NULL;
    actual->sig = nodo;
    return true;
}

//punto 2
int arreglo_reducir(const int arreglo[], size_t n, int (*f)(int a, int b)){
    if(n == 1) return arreglo[0];
    int resultado = f(arreglo[1], arreglo[0]);
    for(size_t i = 3; i < n; i+=2){
        int res_funcion = f(arreglo[i], arreglo[i-1]);
        resultado = f(resultado, res_funcion);
    }
    return resultado;
}

int sumar(int a, int b){
    return a+b;
}

int multiplicar(int a, int b){
    return a*b;
}

//punto 3
size_t caracteres(const char *cadena, char caracter){
    if(*cadena == '\0') return 0;
    if(*cadena == caracter) return caracteres(cadena+1, caracter) + 1;
    return caracteres(cadena+1, caracter);
}

//main de pruebas
int main(void){
    //prueba punto 2
    int arreglo[] = {1,2,4,6};
    printf("La suma es %d\n", arreglo_reducir(arreglo,4,sumar));
    printf("El producto es %d\n", arreglo_reducir(arreglo,4,multiplicar));

    //prueba punto 3
    const char *frase = "botella de alcohol";
    printf("Tiene %zd letras l.\n", caracteres(frase, 'l'));
    printf("Tiene %zd letras z.\n", caracteres(frase, 'z'));

    return 0;
}