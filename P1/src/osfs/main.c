#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"

int main(int argc, char **argv)
{
    // Montar disco
    //os_mount()


    os_mount(argv[1], atoi(argv[2]));
    
    // disk -> mbt->entradas[10][0] = 0b10001010;

    //Testeo de lectura de disco real
    //PAra comparar, se puede correr la linea hexdump -n100 simdiskformat.bin, y se puede ver que los bytes son los mismos
    // int c;
    // //Ponemos el puntero al inicio del archivo
    // fseek(disk -> file_pointer,0, SEEK_SET);
    // for (int i = 0; i <=100; i++){
    //     c = fgetc(disk -> file_pointer);
    //     printf("%x ", c); 

    // }
    // printf("\n");

    if (is_partition_valid(disk -> mbt, 10)){
        printf("Partition %i is valid", 10);
    }

    //Test os_exists
    printf("os_exists\n");
    printf("Archivo hola.txt existe?: %i\n", os_exists("Hola.txt"));

    //Test os_ls
    printf("\n>ls\n");
    os_ls();

    printf("\nDelete partition:\n");

    os_delete_partition(disk -> mbt, 10);


    /*Cerrar archivo*/
    fclose(disk->file_pointer);

    /*FREE*/
    free(disk -> mbt);
    free(disk);
    

    return 0;
}
