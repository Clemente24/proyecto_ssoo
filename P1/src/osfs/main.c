#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"

int main(int argc, char **argv)
{
    // Montar disco
    //os_mount()

    // Declarar master boot table
    Mbt* mbt;

    // Inicializar master boot table con info leida desde el disco
    mbt = init_mbt();

    mbt->entradas[10][0] = 0b11111011;

    if (is_partition_valid(mbt, 10)){
        printf("Partition %i is valid", 10);
    }

    return 0;
}
