#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"

int main(int argc, char **argv)
{
    Directory prueba = directory_init();
    printf("Tamaño del directorio: %li bytes\n", sizeof(prueba));

    //Hago prints de i, pero prueba trabaja con chars de tamaño 1 byte, printf hace la conversion.
    printf("Verificar validez de entrada 32: %i\n\n", is_valid_directory_entry(prueba, 32));

    printf("modificamos entrada 0\n");

    //Probar modificar entrada
    modify_directory_entry(&prueba,0, "archivo.txt", 0b00000000);



    //Pruebo a ver que imprime
    printf("Imprimimos entrada 0 \n");
    print_entry(prueba, 0);
    //Vemos que en bytes, el resto del archivo tiene 0 escrito
    // printf("%x\n", prueba.structure[0][30]);
    

    return 0;
}
