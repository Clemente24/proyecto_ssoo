#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"

int main(int argc, char **argv)
{
    // Montar disco
    //os_mount()


    os_mount(argv[1], 2);
    
    // disk -> mbt->entradas[10][0] = 0b10001010;

    // Testeo de lectura de disco real
    // PAra comparar, se puede correr la linea hexdump -n100 simdiskformat.bin, y se puede ver que los bytes son los mismos
    int c;

    /* Forma para hacer iterando*/

    //Ponemos el puntero al inicio del archivo
    // fseek(disk -> file_pointer,0, SEEK_SET);
    // for (int i = 0; i <=10; i++){

    //     //Escribimos en el byte por byte
    //     char byte_que_escribo = 0x02;
    //     fseek(disk -> file_pointer,0, SEEK_SET);
    //     fwrite(&byte_que_escribo, sizeof(char), 1, disk -> file_pointer);

    //     //VEmos si se escribio bien
    //     fseek(disk -> file_pointer,0, SEEK_SET);
    //     c = fgetc(disk -> file_pointer);
    //     printf("Al final: %x\n", c); 
        

    // }
    //delete_file(disk->directory,"spike.gif");
    //printf("vamos\n");
    // bitmap_update(15);//0000010000100000
    // bitmap_invalid(15);

    /*** TEST BITMAP ****/
    //os_bitmap(0);

    // Test os_mbt();
    printf("os_mbt()\n");
    os_mbt();

    // Test create_partition();
    os_create_partition(5, 30000);


    if (is_partition_valid(10)){
        printf("Partition %i is valid", 10);
    }

    //Test os_exists
    printf("os_exists\n");
    printf("Archivo hola.txt existe?: %i\n", os_exists("Hola.txt"));

    // //Test os_ls
    //printf("\n>ls: \n");
    os_ls();
    //Test osread

    //test delete
    // os_rm("js.jpg");


    /* Test para probar os_open en modo read y os_read y os_close en modo read*/
    // save_file("pep.jpg");

    //[Test os_write]
    /*osFILE *file_desc = os_open("test69.txt", 'w');
    int nbytes = 1200;
    //char *buffer[21] = {0x48, 0x6f, 0x6c, 0x61, 0x20, 0x65, 0x73, 0x74, 0x65, 0x20, 0x65, 0x73, 0x20, 0x75, 0x6e, 0x20, 0x74, 0x65, 0x78, 0x74, 0x6f};
    char *buffer = malloc(sizeof(unsigned char *) * 2100);
    os_write(file_desc, buffer, nbytes);
    free(file_desc);*/

    //os_ls();
    //save_file("test69.txt");

    // Test create file inside disk WORKS:
    // int ubicacion = create_file(disk -> directory, 57997, "primer_archivo.txt");
    // printf("Guardamos el archivo en el indice %i del directorio \n", ubicacion);

    //Test delete file inside disk:
    // delete_file(disk -> directory, "Como_Estamos.txt");

    // printf("\nDelete partition:\n");

    //Test delete_partition
    // os_delete_partition(10);


    /*Cerrar archivo*/
    fclose(disk->file_pointer);

    /*FREE*/
    free(disk -> mbt);
    free(disk);
    

    return 0;
}

//BITMAP DESPUES DE BORRAR
//Bloques LIBRES 65257
// /Bloques OCUPADOS 279 (Se borraron 146 + 1 bloques correctamente, el +1 es por el bloque indice :D)

//TAmano archivo js.jpg en bytes : 24985, en bloques : 13
//BITMAP ANTES DE BORRAR
// Bloques LIBRES 121685
// Bloques OCUPADOS 1771

//BITMAP DESPUES DE BORRAR
// Bloques LIBRES 121699
// Bloques OCUPADOS 1757 = (1771 - 13 - 1 = 1757, el -1 es porque tambien eliminamos el bloque indice :D)
