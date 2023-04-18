#include "color.h"

color_t color_sumar(color_t c, color_t m, float p){
    color_t suma;
    suma.r = c.r + p * m.r;
    suma.g = c.g + p * m.g;
    suma.b = c.b + p * m.b;
    return suma;
}

color_t color_absorber(color_t b, color_t c){
    color_t absorcion;
    absorcion.r = b.r * c.r;
    absorcion.g = b.g * c.g;
    absorcion.b = b.b * c.b;
    return absorcion;
}

color_t color_limitar(color_t a){
    color_t c = a;
    if(c.r < 0) c.r = 0;
    if(c.g < 0) c.g = 0;
    if(c.b < 0) c.b = 0;
    if(c.r > 1) c.r = 1;
    if(c.g > 1) c.g = 1;
    if(c.b > 1) c.b = 1;
    return c;
}

