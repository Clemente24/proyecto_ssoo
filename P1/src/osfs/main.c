#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"

int main(int argc, char **argv)
{
    // Montar disco
    //os_mount()


    os_mount(argv[1], atoi(argv[2]));
    
    disk -> mbt->entradas[10][0] = 0b10001010;

    if (is_partition_valid(disk -> mbt, 10)){
        printf("Partition %i is valid", 10);
    }

    os_delete_partition(disk -> mbt, 10);


    /*Cerrar archivo*/
    fclose(disk->file_pointer);

    /*FREE*/
    free(disk -> mbt);
    free(disk);
    

    return 0;
}
