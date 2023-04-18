#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//punto 1
#define MASK_DER 0x80
#define MASK_GIRO 0x38
#define SHIFT_GIRO 3
bool girar_derecha(uint8_t *r){
    uint8_t aumento_giro = ((*r & MASK_GIRO) >> SHIFT_GIRO) + 1;
    if(!(*r & MASK_DER)){ // DER:1
        if(aumento_giro > 7)
            return false;
        *r |= (aumento_giro << SHIFT_GIRO);
        return true;
    }
    uint8_t decremento_giro = ((*r & MASK_GIRO) >> SHIFT_GIRO) - 1; //DER:0
    if(decremento_giro < 0)
        return false;
    *r |= (decremento_giro << SHIFT_GIRO);
    return true;
}

//punto 2
char *join(const char **ss, char sep){
    size_t i, longitud = 0;
    for(i = 0; ss[i]; i++){
        longitud += strlen(ss[i]);
    }
    char *cadena = malloc((longitud + i) * sizeof(char));
    if(!cadena) return NULL;
    int offset = 0;
    for(size_t k = 0; k < i; k++){
        for(size_t j = offset; j < longitud+i; j++){
                cadena[j] = ss[k][j];
        }
        offset += strlen(ss[k]);
        cadena[strlen(ss[k])] = sep;
    }
    cadena[longitud+i-1] = '\0';
    return cadena;
}

void liberar_cadena(char *cadena){
    free(cadena);
}

//punto 3
#define MEMORIA 50
int *generar_ruido(int maximo, size_t *n){
    size_t generados = MEMORIA;
    int *ruido = malloc(generados * sizeof(int));
    if (!ruido) return NULL;
    size_t i;
    for(i = 0; (ruido[i] = rand()) < maximo; i++){
        if(i == generados - 1){
            generados += MEMORIA;
            int *aux = realloc(ruido, generados * sizeof(int));
            if(!aux){
                free(ruido);
                return NULL;
            }
            ruido = aux;
        }
    }
    *n = i;
    return ruido;
}

//main de pruebas
int main(void){
    //prueba punto 1
    uint8_t registro = 0x8C;
    bool hay_giro = girar_derecha(&registro); //-> (DER: 1, G: 7);
    printf("%s aumento el giro a la derecha\n", hay_giro ? "Se" : "No se");

    //prueba punto 2
    const char *ss[] = {
            "Hola",
            "que",
            "tal",
            NULL
    };
    char *v = join(ss, '|');
    printf("%s\n", v);
    liberar_cadena(v);

    //prueba punto 3
    size_t n = 0;
    int *ruido = generar_ruido(RAND_MAX - 500000, &n);
    printf("se almacenaron %zd numeros\n", n);
    free(ruido);
    return 0;
}