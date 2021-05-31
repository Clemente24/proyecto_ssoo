#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"

int main(int argc, char **argv)
{
    Directory prueba = directory_init(10);
    printf("Tamaño del directorio: %li bytes\n", sizeof(prueba));

    //Hago prints de i, pero prueba trabaja con chars de tamaño 1 byte, printf hace la conversion.
    printf("\nVerificar validez de entrada 32: %i\n", is_valid_directory_entry(prueba, 32));

    printf("\nmodificamos entrada 0\n");

    //Probar modificar entrada
    // modify_directory_entry(&prueba,0, "archivo.txt", 0b00000000);



    //Pruebo a ver que imprime
    printf("\nImprimimos entrada 0 \n");
    print_entry(prueba, 0);

    //Imprimimos solo el nombre, sirve para ls:
    printf("\nImprimimos solo el nombre del archivo en la entrada 0 \n");
    
    //Creamos la variable a la cual le queremos asignar los valores
    char nombre[28];
    nombre_archivo(prueba, 0, nombre);
    printf("%s\n", nombre);
    //Vemos que en bytes, el resto del archivo tiene 0 escrito
    // printf("%x\n", prueba.structure[0][30]);

    /* COmo entiendo que funciona:
        Bloque bitmap recorre todos los bloques y crea un array que indica cual bloque es valido y cual no segun la posicion en el array
        Bloque directorio tiene nombre de archivo y direccion relativa de bloque indice
            -> Bloque indice tiene EL tamaño del archivo y direcciones relativas a bloques de datos
            -> Bloque de datos tiene la informacion binaria final */
    

    return 0;
}
