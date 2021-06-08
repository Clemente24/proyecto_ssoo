#include <stdio.h>
#include <stdlib.h>
#include"../Utilizacion_API/os_API.h"
#include"../Utilizacion_API/directory.h"
#include"../Utilizacion_API/bloqueindice.h"

int main(int argc, char **argv)
{
    /****** TEST DE MONTAR y LS **************/
    // printf("monto el disco\n");
    //Cambiar el id de la particion aca, en el disco filled, las particiones validas en el filled son 2, 3 y 4
    //Si se usan particiones invalidas, el directorio no funciona bien, pq las particiones invalidas tienen tamaños de bloque que no tienen sentido
    
    //Particion 2:
    //os_mount(argv[1], 2);
    // printf("> ls\n");
    // os_ls();



    //Test Exists
    // printf("os_exists\n");
    // printf("Archivo nene.txt existe? : %i\n", os_exists("nene.txt"));
    // printf("Archivo mercedes_benz.mp4 existe? : %i\n", os_exists(" mercedes_benz.mp4"));

    //PArticion 3:
    // os_mount(argv[1], 3);
    // printf("> ls\n");
    // os_ls();

    //Particion 4: 
    // os_mount(argv[1], 4);
    // printf("> ls\n");
    // os_ls();

    

    /************ TEST DE BITMAP  *****************/
    // os_mount(argv[1], 2);
    // os_bitmap(0);


    
    /******************** Test de remove *****************/
    // os_mount(argv[1], 2);
    // printf(">ls \n");
    // os_ls();
    // osFILE* archivo_a_borrar =  os_open("visage.png", 'r');
    // printf(" Tamano de archivo en bytes: %lu \n", archivo_a_borrar -> size);
    // printf(" Tamano de archivo en bloques: %lu \n", (unsigned long int) ceil(archivo_a_borrar -> size/2048.0));
    // os_close (archivo_a_borrar);

    // printf("Bitmap\n");
    // os_bitmap(0);
    // printf("> os_rm\n");
    // os_rm("visage.png");
    // printf("Bitmap\n");
    // os_bitmap(0);
    // printf(">ls \n");
    // os_ls();

    //Si se borra Visage.png
    // TAmano 77
    // bloques ocupados 1767
    // bloques ocupados al final 1689
    // 1767 - 1689 = 78 pq es bloque indice + tamano del archivo en bloques
    // Bloques LIBRES 121675
    // Bloques OCUPADOS 1781


    /************************ TEst de open / read(Dentro de save)******************************/
    // os_mount(argv[1], 2);
    // printf(">ls \n");
    // os_ls();
    // printf("Guardamos el archivo\n");
    // save_file("wtm.jpg");


    /************************ Test de write ************************************/
    // os_mount(argv[1], 2);
    // osFILE *file_desc = os_open("test73.txt", 'w');
    // int nbytes = 10;
    // char* buffer = "hola mundo";
    // printf("os_write\n");
    // os_write(file_desc, buffer, nbytes);
    // printf("os_close\n");
    // os_close(file_desc);
    // printf(">os_ls\n");
    // os_ls();
    // printf("save test 73\n");
    // save_file("test73.txt");




    /*********************** Test de os_mbt y delete_partition **********************/
    // os_mount(argv[1], 2);
    // printf("os_mbt\n");
    // // printf("Get partition index: %i\n", get_partition_index(4));
    // os_mbt();
    // printf("Delete partition 10\n");
    // os_delete_partition(1);
    // printf("os_mbt\n");
    // os_mbt();


    /*********************** Test de reset mbt *********************************/
    // printf("os_mbt\n");
    // os_mbt();
    // printf("reset_mbt\n");
    // os_reset_mbt();
    // printf("os_mbt\n");
    // os_mbt();



    /*Cerrar archivo*/
    fclose(disk->file_pointer);

    /*FREE*/
    free(disk -> mbt);
    free(disk);
    

    return 0;
}

//BITMAP DESPUES DE BORRAR
//Bloques LIBRES 65257
// /Bloques OCUPADOS 279 (Se borraron 146 + 1 bloques correctamente, el +1 es por el bloque indice :D)

//TAmano archivo js.jpg en bytes : 24985, en bloques : 13
//BITMAP ANTES DE BORRAR
// Bloques LIBRES 121685
// Bloques OCUPADOS 1771

//BITMAP DESPUES DE BORRAR
// Bloques LIBRES 121699
// Bloques OCUPADOS 1757 = (1771 - 13 - 1 = 1757, el -1 es porque tambien eliminamos el bloque indice :D)
