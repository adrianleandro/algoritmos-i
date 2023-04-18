#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *nombre;
    int padron;
} alumno_t;

// HACER: implementar las funciones

alumno_t *alumno_crear(const char *nombre, int padron) {
    alumno_t *alumno = malloc(sizeof(alumno_t));
    if(!alumno) return NULL;
    alumno->padron = padron;
    size_t n = strlen(nombre);
    alumno->nombre = malloc((n+1) * sizeof(char));
    if(!alumno->nombre){
        free(alumno);
        return NULL;
    }
    strcpy(alumno->nombre, nombre);
    return alumno;
}

void alumno_destruir(alumno_t *a) {
    free(a->nombre);
    free(a);
}

alumno_t *buscar_alumno(char *nombre, alumno_t *alumnos[], size_t n) {
    size_t izq = 0;
    size_t der = n - 1;
    while(izq <= der){
        size_t piv = (izq+der)/2;
        int res = strcmp(alumnos[piv]->nombre, nombre);
        if(res == 0)
            return alumnos[piv];
        if(res < 0){
            izq = piv + 1;
        } else { //res > 0
            der = piv - 1;
        }
    }
    return NULL;
}

int main(void) {
    char nombre[] = "sebastian";

    alumno_t *sebas = alumno_crear(nombre, 123456);
    assert(sebas != NULL);
    // la estructura debe guardar una copia de la cadena
    assert(sebas->nombre != nombre);

    alumno_t *luca = alumno_crear("luca", 123457);
    alumno_t *santi = alumno_crear("santi", 123458);
    alumno_t *juli = alumno_crear("juli", 123459);

    alumno_t *alumnos[] = {juli, luca, santi, sebas};

    assert(buscar_alumno("luca", alumnos, 4) == luca);

    alumno_destruir(juli);
    alumno_destruir(luca);
    alumno_destruir(santi);
    alumno_destruir(sebas);

    // OPCIONAL: escribir pruebas adicionales

    printf("%s: OK\n", __FILE__);
    return 0;
}
