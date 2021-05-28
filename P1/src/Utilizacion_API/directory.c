#include"directory.h"
#include "os_API.h"

Directory directory_init(int ubicacion_bytes_particion){
    
    //Asumiendo que la ubicacion particion es la entrada 0, leo los primeros 8 bytes del disco:
    unsigned char particion_montada_directory[8]; /* ES CLAVE USAR UNSIGNED LOL*/
    //Array para guardar pos absoluta de directorio
    unsigned char posicion_absoluta_directorio[3];

    fseek(disk -> file_pointer,8 * ubicacion_bytes_particion, SEEK_SET);
    //Leo 8 bytes desde la ubicacion_bytes_particion
    fread(particion_montada_directory, sizeof(char), 8, disk -> file_pointer);

    //Leo los 3 bytes que indican la posicion relativa(Sumo 1 por que parte del segundo byte):
    fseek(disk -> file_pointer,(8 * ubicacion_bytes_particion) + 1, SEEK_SET);
    fread(posicion_absoluta_directorio, sizeof(char), 3, disk -> file_pointer);

    //Debug
    // for (int i = 0; i<8; i++){
    //     printf("%x", particion_montada_directory[i]);
    // }
    // printf("\n");

    // for (int i = 0; i<3; i++){
    //     printf("%x", posicion_absoluta_directorio[i]);
    // }


    //Transformamos la posicion a un int: Nos deberia dar 50
    unsigned long int pos_bloque_directorio = (posicion_absoluta_directorio[0]<<16)|(posicion_absoluta_directorio[1]<<8)|posicion_absoluta_directorio[2];
    printf("\nPOS DIRECTORIO: %lu\n", pos_bloque_directorio);
    //Pos en bytes es: 1KB MBT + 2KB * POS ABSOLUTA
    unsigned long int directory_byte_pos = 1024 + 2 * 1024 * pos_bloque_directorio;
    printf("POS DIRECTORIO EN BYTES: %lu", directory_byte_pos);
    printf("\n");


    /* TEST */
    //Ubicamos el puntero en el byte inicial del bloque directorio
    // fseek(disk -> file_pointer,directory_byte_pos, SEEK_SET);

    // //Creamos el array temporal
    // unsigned char entrada[32];

    // fread(entrada, sizeof(char), 32, disk -> file_pointer);

    // //Debug WORKS :D
    // printf("Entrada (TEST): \n");
    // for (int i = 0; i<32; i++){
    //     printf("%x ", entrada[i]);
    // }
    // printf("\n");


    Directory directory = {};

    //Ubicamos el puntero en la ubicacion del primer byte del bloque directorio
    fseek(disk -> file_pointer,directory_byte_pos, SEEK_SET);
    for (int i = 0;i < 64; i++){
        //Creamos el array temporal
        unsigned char entrada[32];
        //Leemos cada entrada de 32 bytes
        fread(entrada, sizeof(char), 32, disk -> file_pointer);
        //ASignamos los characters a la estructura
        for (int j = 0; j<32; j++){
            directory.structure[i][j] = entrada[j];
        }
    }

    //DEBUG, imprimimos todo el bloque 
    // printf("Bloque Directorio: \n");
    // for (int j = 0;j < 64; j++){
    //     for (int i = 0; i<32; i++){
    //         printf("%x ", directory.structure[j][i]);
    //     }
    //     printf("\n");
    // }

    printf("\n");

    /*Metodo con malloc, creo que no es bueno pq un pointer tiene tamaño 8 bytes, esta manera no respetaria el tamaño de 2KB deseados.*/
    // Directory* directory = malloc(sizeof(Directory));
    // directory = (Directory){
        
    //     //Verificar si hacer malloc de 64 entradas altiro, o haciendo realloc al momento que se agrega una entrada
    //     .array = (char**)malloc(64 * sizeof(char*)),
    // };

    // for(int i = 0; i < 64; i++){
    //     directory -> array[i] = (char*)malloc(32 * sizeof(char));
    // }
    return directory;
};

int is_valid_directory_entry(Directory directory, int index){
    if(index > 64 || index < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return 0;
    }
    //DEBUG WORKS
    // printf("Index %i valid byte: %i\n",index, directory.structure[index][0]);
    if(directory.structure[index][0] == 0){
        return 0;
    }else{
        return 1;
    }

};

int modify_directory_entry(Directory* directory, int index, char* filename, char bit_validez){
    if(index > 64 || index < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return 1;
    }

    if(strlen(filename) > 28){
        printf("Nombre de archivo excede la cantidad maxima de caracteres: 28\n");
        return 1;
    }

    //Obtener el numero relativo el directorio, por ahora no se como o cual es su relacion con el disco
    //por ahora definimos un numero random
    // int file_index = 123;

    //Por ahora escribo ints, pero en algun momento tendre que escribir bytes a un archivo.
    // sprintf(directory -> structure[index], "%c%c%c%c%s", bit_validez, 0b00000000, 0b00000001, 0b00000000, filename);

    //Pruebo a ver el tamaño:
    // printf("Entrada de tamaño: %li bytes\n\n", sizeof(directory -> structure[0]));

    return 0;
    


};

int get_index_relative_position(Directory directory, int index){
    if (is_valid_directory_entry(directory, index)){

        unsigned long int pos_relativa_archivo = (directory.structure[index][1]<<16)|(directory.structure[index][2]<<8)|directory.structure[index][3];
        printf("\nPOS Relativa de archivo: %lu\n", pos_relativa_archivo);
        return pos_relativa_archivo;
    }
    return -1;

}

int nombre_archivo(Directory directory, int index, char nombre[28]){
    if(index > 64 || index < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return 1;
    }
    // char nombre_archivo[28];

    for (int i=0; i<28; i++){
        nombre[i] = directory.structure[index][i+4];
    }
    /*Se puede modificar para que el formato de la funcion ls de la api sea mejor*/
    // printf("%s\n", nombre_archivo);

    return 0;

}

int print_entry(Directory directory, int index){
    if(index > 64 || index < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return 1;
    }
    printf("valid byte: %i\n", directory.structure[index][0]);

    //Concatenamos los 3 bytes para obtener el numero de indice:
    int relative_id = 0;


    relative_id = (directory.structure[index][1] << 16) | (directory.structure[index][2] << 8) | (directory.structure[index][3]);
    printf("relative id: %i\n", relative_id);


    //Imprimimos el nombre del archivo
    // print_nombre_archivo(directory,index);

    //No es necesario agregar 0 manualmente, eso ya se hizo al declarar structure[64][32]
    // printf("Imprimir caracter 30, %i\n", nombre_archivo[27]);

    
    return 0;
}