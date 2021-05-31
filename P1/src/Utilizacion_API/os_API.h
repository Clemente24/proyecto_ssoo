#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include"directory.h"
// Tells the compiler to compile this file once
#pragma once

/* Structs*/
typedef struct mbt
{
    unsigned char entradas[128][8];
  
} Mbt;

typedef struct disk
{
    char* name;
    Mbt* mbt;
    Directory directory;
    FILE* file_pointer;
  
} Disk;

typedef struct osFILE {
    char name[28];
    unsigned int index_ptr;
    unsigned int directory_ptr;
    unsigned int size;
    unsigned int bytes_read;
}osFILE;


/* Variables globales */

Mbt* mbt;
Disk *disk;
unsigned char * particion_montada;


// Functions
void os_mount(char* diskname, int partition);
Disk* init_disk(char* filename);

// MBT functions 
Mbt* init_mbt(FILE* fp);
int is_partition_valid(int indice);
int get_partition_id(int indice);
int get_partition_size(int indice);
int get_partition_block_id(int indice);
int os_delete_partition(int id);
int os_reset_mbt();
int os_mbt();
int os_create_partition(int id, int size);

/*Función para ver si un archivo existe. Retorna 1 si el archivo existe
y 0 en caso contrario. */
int os_exists(char* filename);

/*Función para listar los elementos de la partición actual. Imprime en pantalla los nombres de
todos los archivos.*/
void os_ls();
/*Funcion para hacer update del bitmap en el bloque nro block*/
int bitmap_update(int block);
void os_bitmap(unsigned block);
int available_directory(int ptr);
int available_block();
unsigned int file_data(unsigned int pt);

osFILE * os_open(char * filename,char mode);