#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
// Tells the compiler to compile this file once
#pragma once

char * particion_montada;

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

// Functions
void os_mount(char* diskname, int partition);
Disk* init_disk(char* filename);

Mbt* init_mbt();
int is_partition_valid(Mbt* mbt, int index);

int os_delete_partition(Mbt* mbt,int id);
