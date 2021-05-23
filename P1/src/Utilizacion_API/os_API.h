#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
// Tells the compiler to compile this file once
#pragma once

/* Structs*/
typedef struct mbt
{
    char entradas[128][8];
  
} Mbt;

typedef struct disk
{
    char* name;
    Mbt* mbt;
    FILE* file_pointer;
  
} Disk;

/* Variables globales */

Mbt* mbt;
Disk *disk;
char * particion_montada;


// Functions
void os_mount(char* diskname, int partition);
Disk* init_disk(char* filename);

Mbt* init_mbt();
int is_partition_valid(Mbt* mbt, int index);

int os_delete_partition(Mbt* mbt,int id);

/*Función para ver si un archivo existe. Retorna 1 si el archivo existe
y 0 en caso contrario. */
int os_exists(char* filename);