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
    char *name;
    Mbt *mbt;
    FILE *file_pointer;

} Disk;

typedef struct osFILE
{
    char name[28];
    unsigned int index_ptr;
    unsigned int directory_ptr;
    unsigned int size;
    unsigned int bytes_read;
} osFILE;

/* Variables globales */

Mbt *mbt;
Disk *disk;
char *particion_montada;

// Functions
void os_mount(char *diskname, int partition);
Disk *init_disk(char *filename);

Mbt *init_mbt();
int is_partition_valid(Mbt *mbt, int index);

int os_delete_partition(Mbt *mbt, int id);

/*Función para ver si un archivo existe. Retorna 1 si el archivo existe
y 0 en caso contrario. */
int os_exists(char *filename);

/*Función para listar los elementos de la partición actual. Imprime en pantalla los nombres de
todos los archivos.*/
void os_ls();

/*Funcion para escribir en un archivo*/
osFILE *init_osfile(unsigned int size);
int os_write(osFILE *file_desc, void *buffer, int nbytes);
