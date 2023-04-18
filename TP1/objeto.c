#include "objeto.h"

static objeto_t *objeto_crear(forma_t forma, void *geom, color_t color, float ka, float kd, float ks, float kr){
    objeto_t *objeto = malloc(sizeof(objeto_t));
    if(!objeto) return NULL;
    objeto->forma = forma;
    objeto->color = color;
    objeto->geometria = geom;
    objeto->ka = ka;
    objeto->kd = kd;
    objeto->ks = ks;
    objeto->kr = kr;
    return objeto;
}

objeto_t *triangulo_crear(vector_t normal, vector_t p0, vector_t p1, vector_t p2, color_t color, float ka, float kd, float ks, float kr){
    triangulo_t *triangulo = malloc(sizeof(triangulo_t));
    if(!triangulo) return NULL;
    triangulo->normal = normal;
    triangulo->p0 = p0;
    triangulo->p1 = p1;
    triangulo->p2 = p2;
    objeto_t *obj = objeto_crear(TRIANGULO, triangulo, color, ka, kd, ks, kr);
    if(!obj){
        free(triangulo);
        return NULL;
    }
    return obj;
}

objeto_t *esfera_crear(vector_t centro, float radio, color_t color, float ka, float kd, float ks, float kr){
    esfera_t *esfera = malloc(sizeof(esfera_t));
    if(!esfera) return NULL;
    esfera->centro = centro;
    esfera->radio = radio;
    objeto_t *obj = objeto_crear(ESFERA, esfera, color, ka, kd, ks, kr);
    if(!obj){
        free(esfera);
        return NULL;
    }
    return obj;
}

objeto_t *plano_crear(vector_t normal, vector_t p0, color_t color, float ka, float kd, float ks, float kr){
    plano_t *plano = malloc(sizeof(plano_t));
    if(!plano) return NULL;
    plano->normal = normal;
    plano->p0 = p0;
    objeto_t *obj = objeto_crear(PLANO, plano, color, ka, kd, ks, kr);
    if(!obj){
        free(plano);
        return NULL;
    }
    return obj;
}

void objeto_destruir(void *obj){
    objeto_t *objeto = obj;
    if(objeto->forma == MALLA){
        malla_t *malla = objeto->geometria;
        for(size_t i = 0; i < malla->n; i++)
            objeto_destruir(malla->malla[i]);
        free(malla->malla);
    }
    free(objeto->geometria);
    free(objeto);
}

objeto_t *malla_crear(const char *ruta, vector_t posicion, float escala, color_t color, float ka, float kd, float ks, float kr){
    uint32_t n;
    FILE *stl = fopen(ruta, "rb");
    if(!stl) return false;
    char buff[80];
    fread(buff,sizeof(char), 80, stl);
    fread(&n, sizeof(uint32_t), 1, stl);
    float normal[3], p0[3], p1[3], p2[3];
    malla_t *malla = malloc(sizeof(malla_t));
    if(!malla) {
        fclose(stl);
        return NULL;
    }
    malla->n = n;
    malla->malla = malloc(n * sizeof(triangulo_t *));
    if(!malla->malla){
        fclose(stl);
        free(malla);
        return NULL;
    }
    for(size_t i = 0; i < malla->n; i++){
        fread(&normal, sizeof(float), 3, stl);
        fread(&p0, sizeof(float), 3, stl);
        fread(&p1, sizeof(float), 3, stl);
        fread(&p2, sizeof(float), 3, stl);
        vector_t norm = vector_crear(normal[0], normal[1], normal[2]);
        vector_t punto0 = vector_interpolar_recta(posicion, vector_crear(p0[0], p0[1], p0[2]), escala);
        vector_t punto1 = vector_interpolar_recta(posicion, vector_crear(p1[0], p1[1], p1[2]), escala);
        vector_t punto2 = vector_interpolar_recta(posicion, vector_crear(p2[0], p2[1], p2[2]), escala);
        //if (i == 0)printf("triangulo %zd:\n p0 = (%f %f %f) \n p1 = (%f %f %f) \n p2 = (%f %f %f) \n", i, punto0.x, punto0.y, punto0.z, punto1.x, punto1.y, punto1.z, punto2.x, punto2.y, punto2.z);
        malla->malla[i] = triangulo_crear(norm, punto0, punto1, punto2, color, ka, kd, ks, kr);
        if(!malla->malla[i]){
            for(size_t j = 0; j < i; j++){
                objeto_destruir(malla->malla[j]);
            }
            free(malla);
            fclose(stl);
            return NULL;
        }
        fread(buff, sizeof(uint16_t), 1, stl);
    }
    fclose(stl);
    objeto_t *obj = objeto_crear(MALLA, malla, color, ka, kd, ks, kr);
    if(!obj){
        for(size_t j = 0; j < malla->n; j++){
            objeto_destruir(malla->malla[j]);
        }
        free(malla);
        return NULL;
    }
    return obj;
}

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual){
    luz_t *luz = malloc(sizeof(luz_t));
    if(!luz)
        return NULL;
    luz->posicion = posicion;
    luz->color = color;
    luz->es_puntual = es_puntual;
    return luz;
}

void luz_destruir(void *luz){
    free(luz);
}

static float distancia_esfera(void *forma, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    esfera_t *esfera = forma;
    vector_t origen_centro = vector_resta(esfera->centro,o); // origen_centro: C - O
    float beta = vector_producto_interno(origen_centro,d); // beta: (C - O) * d
    float delta = beta * beta - vector_producto_interno(origen_centro, origen_centro) + esfera->radio * esfera->radio; // delta: beta**2 - (C-O).(C-O) + R**2
    if(delta < 0)
        return INFINITO;
    float sqrt_delta = sqrt(delta);
    float t1 = beta + sqrt_delta;
    float t2 = beta - sqrt_delta;
    if(t1 < 0 && t2 < 0)
        return INFINITO;
    float t;
    if(t1 <= t2)
        t = t1;
    else
        t = t2;
    *punto = vector_interpolar_recta(o,d,t);
    *normal = vector_normalizar(vector_resta(*punto, esfera->centro));
    return t;
}

static float distancia_plano(void *forma, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    plano_t *plano = forma;
    float dn = vector_producto_interno(d,plano->normal);
    if(dn == 0) return INFINITO;
    float t = vector_producto_interno(vector_resta(plano->p0, o),plano->normal) / dn;
    if(t < 0) return INFINITO;
    *punto = vector_interpolar_recta(o, d, t);
    *normal = plano->normal;
    return t;
}

static float distancia_triangulo(void *forma, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    triangulo_t *triangulo = forma;
    vector_t e1 = vector_resta(triangulo->p1, triangulo->p0);
    vector_t e2 = vector_resta(triangulo->p2, triangulo->p0);
    vector_t h = producto_vectorial(d, e2);
    float a = vector_producto_interno(e1, h);
    if(fabs(a) < EPSILON) return INFINITO;
    vector_t s = vector_resta(o, triangulo->p0);
    float u = vector_producto_interno(s, h) / a;
    if(u < 0 || u > 1) return INFINITO;
    vector_t q = producto_vectorial(s, e1);
    float v = vector_producto_interno(d, q) / a;
    if(v < 0 || u + v > 1) return INFINITO;
    float t = vector_producto_interno(e2, q) / a;
    if(t < 0) return INFINITO;
    *punto = vector_interpolar_recta(o,d,t);
    *normal = triangulo->normal;
    return t;
}

static float distancia_malla(void *forma, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    malla_t *malla = forma;
    size_t minima;
    float t = INFINITO;
    for(size_t i = 0; i < malla->n; i++){
        float dist = distancia_triangulo(((objeto_t *)malla->malla[i])->geometria, o, d, punto, normal);
        //printf("%f\n", dist);
        if(dist < t){
            minima = i;
            t = dist;
        }
    }
    if(t < INFINITO){
        triangulo_t *triangulo = ((objeto_t *)malla->malla[minima])->geometria;
        *punto = vector_interpolar_recta(o, d, t);
        //printf("%f %f %f \n", punto->x, punto->y, punto->z);
        *normal = triangulo->normal;
    }
    return t;
}

typedef float (*distancia_t)(void *forma, vector_t o, vector_t d, vector_t *punto, vector_t *normal);

float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal){
    distancia_t distancias[] = {
            [TRIANGULO] = distancia_triangulo,
            [ESFERA] = distancia_esfera,
            [PLANO] = distancia_plano,
            [MALLA] = distancia_malla
    };
    return distancias[objeto->forma](objeto->geometria, o, d, punto, normal);
}