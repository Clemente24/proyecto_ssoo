#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include"directory.h"
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
    Directory directory;
    FILE* file_pointer;
  
} Disk;

/* Variables globales */

Mbt* mbt;
Disk *disk;
char * particion_montada;


// Functions
void os_mount(char* diskname, int partition);
Disk* init_disk(char* filename);

// MBT functions 
Mbt* init_mbt(FILE* fp);
int is_partition_valid(int indice);
int os_delete_partition(int id);
int os_reset_mbt();
int os_mbt();

/*Función para ver si un archivo existe. Retorna 1 si el archivo existe
y 0 en caso contrario. */
int os_exists(char* filename);

/*Función para listar los elementos de la partición actual. Imprime en pantalla los nombres de
todos los archivos.*/
void os_ls();