#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include"directory.h"

Directory directory_init(){
    
    //MEtodo con memoria del stack: char array[64][32]
    Directory directory = {};

    //Suponemos que toda entrada es invalida hasta que se guarda un archivo.
    for (int i = 0;i < 64; i++){
        directory.structure[i][0] = 0b00000000;
    }

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
    sprintf(directory -> structure[index], "%c%c%c%c%s", bit_validez, 0b00000000, 0b00000001, 0b00000000, filename);

    //Pruebo a ver el tamaño:
    printf("Entrada de tamaño: %li bytes\n\n", sizeof(directory -> structure[0]));

    return 0;
    


};


int print_nombre_archivo(Directory directory, int index){
    if(index > 64 || index < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return 1;
    }
    char nombre_archivo[28];

    for (int i=0; i<28; i++){
        nombre_archivo[i] = directory.structure[index][i+4];
    }
    /*Se puede modificar para que el formato de la funcion ls de la api sea mejor*/
    printf("%s\n", nombre_archivo);

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
    print_nombre_archivo(directory,index);

    //No es necesario agregar 0 manualmente, eso ya se hizo al declarar structure[64][32]
    // printf("Imprimir caracter 30, %i\n", nombre_archivo[27]);

    
    return 0;
}