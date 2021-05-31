#include <stdio.h>
#include <stdlib.h>
#include "../Utilizacion_API/os_API.h"
#include "../Utilizacion_API/directory.h"

int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));

    char byte_que_escribo = 0xFF;
    //fseek(disk->file_pointer, 2048, SEEK_SET);
    //fwrite(&byte_que_escribo, sizeof(char), 1, disk->file_pointer);

    osFILE *new_file = init_osfile(4096);
    int escrito = os_write(new_file, &byte_que_escribo, 512);
    printf("Escrito: %x\n", escrito);

    /*Cerrar archivo*/
    fclose(disk->file_pointer);

    /*FREE*/
    free(disk->mbt);
    free(disk);

    return 0;
}
