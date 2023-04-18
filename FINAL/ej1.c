#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//Precondicion: k es menor a n
void patron_tablero(int n, bool m[n][n], int k) {
    // HACER: implementar la funcion
    int escritos_i = 0;
    for(size_t i = 0; i < n; i++){
        int escritos_j = 0;
        for(size_t j = 0; j < n; j++){
            if (escritos_i >= k) {
                m[i][j] = escritos_j < k;
            } else {
                m[i][j] = escritos_j >= k;
            }
            escritos_j++;
            if(escritos_j == 2*k) escritos_j = 0;
        }
        escritos_i++;
        if(escritos_i == 2*k) escritos_i = 0;
    }
}

int main(void) {

    bool m[10][10];
    patron_tablero(10, m, 3);

    bool p[10][10] = {
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
    };

    assert(memcmp(m, p, sizeof(m)) == 0);

    // OPCIONAL: escribir pruebas adicionales

    bool c[6][6];
    patron_tablero(6, c, 2); // cambio dimensiones y k

    bool q[6][6] = {
            {0, 0, 1, 1, 0, 0},
            {0, 0, 1, 1, 0, 0},
            {1, 1, 0, 0, 1, 1},
            {1, 1, 0, 0, 1, 1},
            {0, 0, 1, 1, 0, 0},
            {0, 0, 1, 1, 0, 0}
    };

    assert(memcmp(c, q, sizeof(q)) == 0);

    printf("%s: OK\n", __FILE__);
    return 0;
}
