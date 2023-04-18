#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932

#define CX  0.5
#define CY  0.75
#define CZ  1.9
#define R   0.8

#define ANCHO 640
#define ALTO 480
#define FOV 90

float producto_interno(float x1, float y1, float z1, float x2, float y2, float z2){
    return x1 * x2 + y1 * y2 + z1 * z2;
}

float norma(float x, float y, float z){
    return sqrt(producto_interno(x,y,z,x,y,z));
}

int intersecta_esfera(float dx, float dy, float dz){
    float normal_c = norma(CX,CY,CZ);
    float producto_c_por_d = producto_interno(CX,CY,CZ,dx,dy,dz);
    return producto_c_por_d * producto_c_por_d - normal_c * normal_c + R * R >= 0;
}

int main(void){
    printf("P1\n%d %d\n", ANCHO, ALTO); //Encabezado
    float z = ANCHO/(2*tan((FOV/2) * (PI/180)));
    for(int y = ALTO/2; y > -ALTO/2; y--) {
        for (int x = -ANCHO/2; x < ANCHO/2; x++) {
            float modulo = norma(x, y, z);
            printf("%d ", intersecta_esfera(x/modulo, y/modulo, z/modulo));
        }
        putchar('\n');
    }
    return 0;
}
