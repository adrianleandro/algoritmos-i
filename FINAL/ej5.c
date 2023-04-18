#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef uint16_t prop_t;
// 0000 0000 0000 0000
// TTTT TTTT TTTT tscn

// HACER: implementar las funciones

#define MASK_NEGRITA 0x0001
void prop_set_negrita(prop_t *reg, bool negrita) {
    *reg = (*reg & ~MASK_NEGRITA) | (negrita & MASK_NEGRITA);
}

bool prop_get_negrita(prop_t *reg) {
    return *reg & MASK_NEGRITA;
}

#define MASK_CURS 0x0002
#define SHIFT_CURS 1
void prop_set_cursiva(prop_t *reg, bool cursiva) {
    *reg = (*reg & ~MASK_CURS) | ((cursiva << SHIFT_CURS) & MASK_CURS);
}

bool prop_get_cursiva(prop_t *reg) {
    return *reg & MASK_CURS;
}

#define MASK_SUBR 0x0004
#define SHIFT_SUBR 2
void prop_set_subrayado(prop_t *reg, bool subrayado) {
    *reg = (*reg & ~MASK_SUBR) | ((subrayado << SHIFT_SUBR) & MASK_SUBR);
}

bool prop_get_subrayado(prop_t *reg) {
    return *reg & MASK_SUBR;
}

#define MASK_TACH 0x0008
#define SHIFT_TACH 3
void prop_set_tachado(prop_t *reg, bool tachado) {
    *reg = (*reg & ~MASK_TACH) | ((tachado << SHIFT_TACH) & MASK_TACH);
}

bool prop_get_tachado(prop_t *reg) {
    return *reg & MASK_TACH;
}

#define MASK_TAM 0xFFF0
#define SHIFT_TAM 4
void prop_set_tamanio(prop_t *reg, unsigned int tamanio) {
    *reg = (*reg & ~MASK_TAM) | ((tamanio << SHIFT_TAM) & MASK_TAM);
}

unsigned int prop_get_tamanio(prop_t *reg) {
    return (*reg & MASK_TAM) >> SHIFT_TAM;
}

int main(void) {
    prop_t reg = 0;

    prop_set_negrita(&reg, false);
    prop_set_cursiva(&reg, true);
    prop_set_tachado(&reg, false);
    prop_set_subrayado(&reg, true);
    prop_set_tamanio(&reg, 16);

    assert(prop_get_negrita(&reg) == false);
    assert(prop_get_cursiva(&reg) == true);
    assert(prop_get_tachado(&reg) == false);
    assert(prop_get_subrayado(&reg) == true);
    assert(prop_get_tamanio(&reg) == 16);

    prop_set_tamanio(&reg, 4095);
    assert(prop_get_tamanio(&reg) == 4095);

    // OPCIONAL: escribir pruebas adicionales

    printf("%s: OK\n", __FILE__);
    return 0;
}
