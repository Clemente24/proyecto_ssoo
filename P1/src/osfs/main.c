#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"
#include"../Utilizacion_API/bloqueindice.h"

int main(int argc, char **argv)
{
    // Montar disco
    //os_mount()

    printf("monto el disco\n");
    //Cambiar el id de la particion aca, en el disco filled, las particiones validas en el filled son 2, 3 y 4
    //Si se usan particiones invalidas, el directorio no funciona bien, pq las particiones invalidas tienen tamaños de bloque que no tienen sentido
    os_mount(argv[1], 2);
    
    // disk -> mbt->entradas[10][0] = 0b10001010;

    // Testeo de lectura de disco real
    // PAra comparar, se puede correr la linea hexdump -n100 simdiskformat.bin, y se puede ver que los bytes son los mismos
    // int c;

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
    bitmap_update(15);//0000010000100000
    os_bitmap(0);
    char* name = "dog.mp3";
    osFILE* os_file=os_open(name,'r');  //osFILE* os_file= 
    printf("printando\n");
    // printf("Nombre  %s",os_file->name);
    // printf("Directory ptr: %d", os_file->directory_ptr);
    // printf("index ptr: %d", os_file->index_ptr);
    // printf('Size: %d', os_file->size);
    /* Forma para hacer con array*/
    //Ponemos el puntero al inicio del archivo
    //fseek(disk -> file_pointer,0, SEEK_SET);
    // char array_de_bytes[6] = {0x80, 0x70, 0x80, 0x90, 0xaa};
    // sprintf(array_de_bytes, "%x%x%x");
    //fwrite(array_de_bytes, sizeof(char), 6, disk -> file_pointer);
    //escribir_bloque_indice(0,15000,1300);
    // fwrite(array_de_bytes, sizeof(char), 6, disk -> file_pointer);

    if (is_partition_valid(10)){
        printf("Partition %i is valid", 10);
    }
    // Test os_mbt();
    printf("os_mbt()\n");
    os_mbt();




    /**** Tests directorio *****/

    //Test is_Valid_entry

    // is_valid_directory_entry(disk -> directory, 0);
    // is_valid_directory_entry(disk -> directory, 1);
    // is_valid_directory_entry(disk -> directory, 2);

    //TEst pos relativa:
    // get_index_relative_position(disk -> directory, 2);
    //TEst pos absoluta con filename:
    printf("Pos absoluta del archivo: %i\n", get_file_index_absolute_ptr(disk -> directory, "dog.mp3"));



    //Test os_exists
    // printf("os_exists\n");
    //Archivo de la particion 2 del disco filled
    printf("Archivo nene.txt existe? : %i\n", os_exists("nene.txt"));

    // //Test os_ls
    printf("\n>ls: \n");
    os_ls();
    //Test osread
    osFILE* file_desc = malloc(sizeof(osFILE));
    *file_desc= (osFILE) {
    .name= "dog.mp3",
    .index_ptr = 117998592,
    .directory_ptr = 10,
    .size = 486242,
    .bytes_read = 0
    };
    unsigned char *buffer = malloc(sizeof(unsigned char) * 2100 );
    

    os_read(file_desc, buffer, 2100);
    free(file_desc);
    free(buffer);
    // Test create file inside disk WORKS:
    // int ubicacion = create_file(disk -> directory, 57997, "primer_archivo.txt");
    // printf("Guardamos el archivo en el indice %i del directorio \n", ubicacion);

    //Test delete file inside disk:
    // delete_file(disk -> directory, "Como_Estamos.txt");

    // printf("\nDelete partition:\n");

    os_delete_partition(10);


    /*Cerrar archivo*/
    fclose(disk->file_pointer);

    /*FREE*/
    free(disk -> mbt);
    free(disk);
    

    return 0;
}
