#include "directory.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>


void os_mount(char* diskname, int partition){
    /* Toma las variables globales declaradas en el header y les asigna valor */
    disk = init_disk(diskname);
    particion_montada = disk -> mbt->entradas[partition];
    disk->directory = directory_init(0);
    
}

Disk* init_disk(char* filename){
    printf("FILENAME: %s\n", filename);
    FILE* fp = fopen(filename, "r+b");
    Disk* disk = malloc(sizeof(Disk));
    *disk = (Disk) {
        .file_pointer = fp,
        .name = filename,
        .mbt = init_mbt(fp),
        .directory =  directory_init()
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
    Mbt* mbt = malloc(sizeof(Mbt));
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < 128; i++){
        fseek(fp, i * 8, SEEK_SET);
        fread(bytes_leidos, sizeof(char), 8, fp);
        // printf("Bytes leidos: %d %d %d %d %d %d %d %d \n", bytes_leidos[0], bytes_leidos[1], bytes_leidos[2], bytes_leidos[3], bytes_leidos[4], bytes_leidos[5], bytes_leidos[6], bytes_leidos[7]);
        for (int j = 0; j < 7; j++){
            // printf("i: %i, j: %i \n", i, j);
            // printf("byte leido en 0: %d \n", bytes_leidos[0]);
            mbt->entradas[i][j] = bytes_leidos[j];
        }
    }
    return mbt;
}

int is_partition_valid(int indice){
    int bit;
    unsigned char number;

    number = disk->mbt->entradas[indice][0];
    bit = number & (1<<7) ? 1 : 0;
   
    return bit;
}

int os_mbt(){

    for (int i = 0; i < 128; i++){
        if (is_partition_valid(i)){
            printf("Particion N°: %i valida. \n", i);
        };
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

int os_exists(char* filename){
    /*Cabe recalcar que se busca solo en la partición actual*/
    /*Esta funcion asume que la estructura de directorio ya está creada en la particion*/
    /*Issue N°208: El unico bloque tipo directorio es el raiz en una particion*/
    
    //Supongo que directorio sera una variable global
    Directory prueba = directory_init(0);
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
    // - Para leer en la consola hago hexdump -C -n 1024 simdiskfilled.bin
    // 	El -C es para mostrar los bytes en el orden que vienen, y segun el enunciado vienen en big endian.

    // -Testeando simdiskfilled: Leo el primer byte: 0x82: 1000 0010 -> Indica que la particion es valida, y el id unico es 2. (Enunciado de la MBT)

    // ID Absoluto del primer bloque: 3Bytes son: 0x00 0x00 0x32 en binario: 0011 0010 = bloque 50.

    // Esto significa que el primer bloque empieza en el byte: 1KB + 2KB*50
    // 1KB es 2^10 Bytes = 1024 Bytes -> EL bloque directorio empieza en el byte 1024 + 2 * 1024 * 50 = 103424

    // Cantidad de bloques de la particion (Byte 5,6,7,8 de la primera entrada de la MBT): 0x00 0x01 0xe2 0x40 en binario:
    // 00000000 00000001 11100010 01000000 = 123456
    // Tambien el numero 0x1e240 es lo mismo y da en decimal = 123456

    // -Para encontrar mi bloque directorio con hexdump hago: hexdump -C -s 103424 -n 2048 simdiskfilled.bin
    // el -s es para saltarme 103423 bytes y el -n 2048 es para mostrar 2048 Bytes. 
    // El -C es para mostrar los bytes en el orden que vienen, y segun el enunciado vienen en big endian.


    //Supongo que directorio sera una variable global
    Directory prueba = directory_init(0);
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