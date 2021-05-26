#include "directory.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>

void os_mount(char *diskname, int partition)
{
    /* Toma las variables globales declaradas en el header y les asigna valor */
    disk = init_disk(diskname);
    particion_montada = disk->mbt->entradas[partition];
}

Disk *init_disk(char *filename)
{
    printf("FILENAME: %s\n", filename);
    Disk *disk = malloc(sizeof(Disk));
    *disk = (Disk){
        .file_pointer = fopen(filename, "r+b"),
        .name = filename,
        .mbt = init_mbt()};
    if (disk->file_pointer == NULL)
    {
        // esta mal
        printf("Filename doesnt exist\n");
        return NULL;
    }
    else
    {
        return disk;
    }
}

Mbt *init_mbt()
{
    Mbt *mbt = malloc(sizeof(Mbt));
    for (int i = 0; i < 128; i++)
    {
        mbt->entradas[i][0] = 0b00000000;
    }
    return mbt;
}

DataBlock *init_data()
{
    DataBlock *data_block = malloc(sizeof(DataBlock));
    *data_block = (DataBlock){
        .datos = ""};
    return data_block;
}

int is_partition_valid(Mbt *mbt, int index)
{
    int bit;
    int number;
    number = mbt->entradas[index][0];
    for (int i = 0; i < 8; i++)
    {
        bit = (number >> i) & 1U;
        printf("Bit at position %i of partition index: %i is: %i \n", i, index, bit);
    }
    return bit;
}

int os_delete_partition(Mbt *mbt, int id)
{
    int identificador;
    int primer_byte_entrada;
    if (id > 127)
    {
        // out of range
        return 1;
    }
    primer_byte_entrada = mbt->entradas[id][0];
    identificador = primer_byte_entrada & ((1 << 7) - 1);
    printf("Entrada es: %d\n", primer_byte_entrada);
    printf("Identificador es: %i\n", identificador);
    primer_byte_entrada = (primer_byte_entrada & ~(1UL << 8)) | (0 << 8);

    return 0;
}

void os_reset_mbt(Mbt *mbt, int id)
{
    int identificador;
    int entrada;
    for (int i = 0; i < 128; i++)
    {
        os_delete_partition(mbt, i);
    }
}

int os_exists(char *filename)
{
    /*Cabe recalcar que se busca solo en la partición actual*/

    //entrada de 8 bytes:
    //printf("%s", particion_montada);

    return 0;
}