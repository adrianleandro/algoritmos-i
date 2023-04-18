#include <stdio.h>

int main(void){
    FILE *fo = fopen("archivo.bin", "wb");
    if(!fo) return 1;
    size_t h = 2;
    fwrite(&h, sizeof(size_t), 1, fo);
    char cad = '9';
    fwrite(&cad, sizeof(char), 1, fo);
    cad = '5';
    fwrite(&cad, sizeof(char), 1, fo);
    h = 1;
    fwrite(&h, sizeof(size_t), 1, fo);
    cad = '.';
    fwrite(&cad, sizeof(char), 1, fo);
    h = 2;
    fwrite(&h, sizeof(size_t), 1, fo);
    cad = '1';
    fwrite(&cad, sizeof(char), 1, fo);
    cad = '1';
    fwrite(&cad, sizeof(char), 1, fo);
    fclose(fo);
    return 0;
}