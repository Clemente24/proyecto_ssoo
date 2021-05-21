#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>

Mbt* init_mbt()
{
    Mbt* mbt = malloc(sizeof(Mbt));
    for (int i = 0; i < 128; i++){
        mbt->entradas[i][0] = 0b00000000;
    }
    return mbt;
}

int is_partition_valid(Mbt* mbt, int index){
    int bit;
    int number;
    number = mbt->entradas[index][0];
    for (int i = 0; i < 8; i++){
        bit = (number >> i) & 1U;
        printf("Bit at position %i of partition index: %i is: %i \n", i, index, bit);
    }
    return bit;
}

void os_delete_partition(Mbt* mbt, int id){
    int identificador;
    int entrada;
    for (int i = 0; i < 128; i++){
        entrada = mbt->entradas[i][0];
        identificador = entrada & ((1 << 7) - 1);
        printf("Entrada es: %d\n", entrada);
        printf("Identificador es: %i\n", identificador);
        if (identificador == id){
            entrada = (entrada & ~(1UL << 8)) | (0 << 8);
            printf("Nueva entrada es: %i\n", entrada);
        }
    }
}