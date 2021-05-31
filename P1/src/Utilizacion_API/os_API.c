#include "directory.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>


void os_mount(char* diskname, int partition){
    /* Toma las variables globales declaradas en el header y les asigna valor */
    disk = init_disk(diskname);
    particion_montada = disk ->mbt->entradas[partition];
    
}

Disk* init_disk(char* filename){
    printf("FILENAME: %s\n", filename);
    FILE* fp = fopen(filename, "r+b");
    Disk* disk = malloc(sizeof(Disk));
    *disk = (Disk) {
        .file_pointer = fp,
        .name = filename,
        .mbt = init_mbt(fp)
    };
    if (disk -> file_pointer == NULL){
        // Si no existe el archivo 
        printf("Filename doesnt exist\n");
        return NULL;
    }
    else
    {
        return disk;
    }

}

Mbt* init_mbt(FILE* fp)
{
    unsigned char bytes_leidos[8];
    Mbt* mbt = calloc(1, sizeof(Mbt));
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < 128; i++){
        fseek(fp, i * 8, SEEK_SET);
        fread(bytes_leidos, sizeof(char), 8, fp);
        // printf("Bytes leidos: %d %d %d %d %d %d %d %d \n", bytes_leidos[0], bytes_leidos[1], bytes_leidos[2], bytes_leidos[3], bytes_leidos[4], bytes_leidos[5], bytes_leidos[6], bytes_leidos[7]);
        for (int j = 0; j < 8; j++){
            // printf("i: %i, j: %i \n", i, j);
            // printf("byte leido en 0: %d \n", bytes_leidos[0]);
            mbt->entradas[i][j] = bytes_leidos[j];
        }
    }
    return mbt;
}

// MBT functions
int is_partition_valid(int indice){
    int bit;
    unsigned char primer_byte;

    primer_byte = disk->mbt->entradas[indice][0];
    bit = primer_byte & (1<<7) ? 1 : 0;
   
    return bit;
}

int get_partition_block_id(int indice){
    unsigned char* entrada = disk->mbt->entradas[indice];
    unsigned long int abs_block_id;

    abs_block_id = (entrada[1] << 16) | (entrada[2] << 8) | (entrada[3]);

    return abs_block_id;
}

int get_partition_id(int indice){
    unsigned char* entrada = disk->mbt->entradas[indice];
    unsigned int partition_id;

    partition_id = entrada[0] & ((1 << 7) - 1);

    return partition_id;
}

int get_partition_size(int indice){
    unsigned char* entrada = disk->mbt->entradas[indice];
    unsigned long int abs_block_id;
    unsigned long int partition_size;
    unsigned int partition_id;
    int partition_valid;

    partition_valid = entrada[0] & (1<<7) ? 1 : 0;
    partition_id = entrada[0] & ((1 << 7) - 1);
    abs_block_id = (entrada[1] << 16) | (entrada[2] << 8) | (entrada[3]);
    partition_size = (entrada[4] << 24) | (entrada[5] << 16) | (entrada[6] << 8) | (entrada[7]);

    printf("[Partition id: %i], VALID?: %i, partition_size: %li, ID_primer_bloque: %li \n", partition_id, partition_valid, partition_size, abs_block_id);

    return partition_size;
}

int os_mbt(){

    for (int i = 0; i < 128; i++){
        if (is_partition_valid(i)){
            printf("Particion con ID: %i valida. \n", get_partition_id(i));
        }
    }
    return 0;
}

int os_delete_partition(int id){
    int identificador_relativo;
    int primer_byte_entrada;
    if (id > 127){
        // out of range
        return 1;
    }
    primer_byte_entrada = disk->mbt->entradas[id][0];
    identificador_relativo = primer_byte_entrada & ((1 << 7) - 1);
    // printf("Entrada es: %d\n", primer_byte_entrada);
    // printf("Identificador es: %i\n", identificador);
    // Cambiamos el primer bit del primer byte de la entrada a 0
    primer_byte_entrada = (primer_byte_entrada & ~(1UL << 8)) | (0 << 8);
    
    return 0;
}

int os_reset_mbt(){
    for (int i = 0; i < 128; i++){
       os_delete_partition(i);
    }
    return 0;

}

int os_create_partition(int id, int size){


    if (is_partition_valid(id)){
        // Partición con ese id ya está tomada
        return 1;
    }
    for (int i = 0; i < 128; i++){
        if (is_partition_valid(i)){
            printf("[ID: %i]Entrada: %i valida, empieza en: %i", get_partition_id(i), i, get_partition_block_id(i));
        }
    }


    return 0;
}

int os_exists(char* filename){
    /*Cabe recalcar que se busca solo en la partición actual*/
    /*Esta funcion asume que la estructura de directorio ya está creada en la particion*/
    /*Issue N°208: El unico bloque tipo directorio es el raiz en una particion*/
    
    //Supongo que directorio sera una variable global
    Directory prueba = directory_init();
    //Creo un directorio simulado por ahora 
    modify_directory_entry(&prueba, 4, filename, 0b00000001);

    for (int i = 0; i<64; i++){
        //Creamos variable en la cual guardamos el nombre
        char nombre_actual[28];
        //Buscamos el nombre
        nombre_archivo(prueba, i, nombre_actual);
        //Retorna solo si la encuentra
        if (strcmp(nombre_actual, filename) == 0){
            return 1;
        }
    }

    return 0;

}

void os_ls(){
    //Supongo que directorio sera una variable global
    Directory prueba = directory_init();
    //Creo muchos directorios para simular por ahora 
    modify_directory_entry(&prueba, 0,"1.png", 0b00000001);
    modify_directory_entry(&prueba, 1,"2.png", 0b00000001);
    modify_directory_entry(&prueba, 2,"3.png", 0b00000001);
    modify_directory_entry(&prueba, 3,"4.png", 0b00000001);
    modify_directory_entry(&prueba, 4,"5.png", 0b00000001);
    modify_directory_entry(&prueba, 5,"6.png", 0b00000001);
    modify_directory_entry(&prueba, 6,"Tesis.docx", 0b00000001);

    for (int i =0; i<64; i++){
        if (is_valid_directory_entry(prueba, i)){
            char nombre_actual[28];
            //Buscamos el nombre
            nombre_archivo(prueba, i, nombre_actual);
            printf("%s\n", nombre_actual);
        }

    }

}