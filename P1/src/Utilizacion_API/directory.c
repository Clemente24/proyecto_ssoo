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

    //Leo los 3 bytes que indican la posicion absoluta(Sumo 1 por que parte del segundo byte):
    fseek(disk -> file_pointer,(8 * ubicacion_bytes_particion) + 1, SEEK_SET);
    fread(posicion_absoluta_directorio, sizeof(char), 3, disk -> file_pointer);

    //Debug
    // for (int i = 0; i<8; i++){
    //     printf("%x ", particion_montada_directory[i]);
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


    Directory directory = {.directory_block_pos = pos_bloque_directorio,
                           .directory_byte_pos = directory_byte_pos};

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

    // printf("\n");

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

int modify_directory_entry(Directory* directory,unsigned long int identificador_relativo_bloque_indice, unsigned long int entrada_del_directorio, char* filename, char bit_validez){
    
    if(entrada_del_directorio > 64 || entrada_del_directorio < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return -1;
    }
    //DEbug estructura antes:
    // printf("Debug estructura antes\n");
    // for (int i = 0; i<32; i++){
    //     printf("%x ", disk -> directory.structure[entrada_del_directorio][i]);
    // }
    // printf("\n");

    if(strlen(filename) > 28){
        printf("Nombre de archivo excede la cantidad maxima de caracteres: 28\n");
        return -1;
    }

    //Transformamos el identificador relativo del bloque indice a valores hexadecimales
    unsigned char bytes[3];

    bytes[0] = identificador_relativo_bloque_indice >> 16 & 0xFF;
    bytes[1] = identificador_relativo_bloque_indice >> 8  & 0xFF;
    bytes[2] = identificador_relativo_bloque_indice & 0xFF;

    //Convertimos el nombre del archivo en un array de bytes
    char filename_bytes[28] = ""; //="" hace que se inicialize con 0s y no con valores basura
    //Debug
    // printf("\nfilename: %s\n length: %li \n", filename, strlen(filename));

    //TRansformamos caracter a int, y luego lo pasamos a hex. sprintf no me funcaa, supongo que por el orden de los bytes
    for (int i = 0; i<strlen(filename); i++){        
        unsigned long int codigo_ascii = (unsigned long int )filename[i];
        filename_bytes[i] = codigo_ascii & 0xFF;
    }


    //DEBUG FILENAME_BYTES
    // printf("DEBUG FILENAME BYTES\n");
    // for(int i = 0; i<28; i++){
    //     printf("%x ", filename_bytes[i]);
    // }
    // printf("\n");


    //Utilizamos el numero de bytes absolutos requeridos para llegar al directorio
    fseek(disk -> file_pointer, disk -> directory.directory_byte_pos + (32 * entrada_del_directorio), SEEK_SET);

    //Primero escribimos el bit validez, el index relativo
    char array_de_bytes[4] = {bit_validez,bytes[0], bytes[1], bytes[2]};
    fwrite(array_de_bytes, sizeof(char), 4, disk -> file_pointer);
    //Y actualizamos la estructura
    disk -> directory.structure[entrada_del_directorio][0] = bit_validez;
    disk -> directory.structure[entrada_del_directorio][1] = bytes[0];
    disk -> directory.structure[entrada_del_directorio][2] = bytes[1];
    disk -> directory.structure[entrada_del_directorio][3] = bytes[2];

    //Luego escribimos los 28 bytes restantes
    fwrite(filename_bytes, sizeof(char), 28, disk -> file_pointer);
    //Actualizamos la estructura
    for (int i = 0; i < 28; i++){
        disk -> directory.structure[entrada_del_directorio][i + 4] = filename_bytes[i];
    }

    //DEBUG POS RELATIVA
    // unsigned long int pos_relativa = bytes[0]<<16 | bytes [1] << 8 | bytes [2];
    // printf("Pos relativa del archivo guardado: %lu\n", pos_relativa);
 
    //DEbug estructura:
    // printf("Debug estructura despues\n");
    // for (int i = 0; i<32; i++){
    //     printf("%x ", disk -> directory.structure[entrada_del_directorio][i]);
    // }

    return 0;
    


};

int modify_entry_valid_byte(Directory directory,unsigned long int entrada_del_directorio, char bit_validez){
    if(entrada_del_directorio > 64 || entrada_del_directorio < 0){
        printf("Indice invalido, Directorios tienen maximo 64 entradas\n");
        return -1;
    }
    //Vamos al bit de validez y lo cambiamos.
    fseek(disk -> file_pointer, disk -> directory.directory_byte_pos + (32 * entrada_del_directorio), SEEK_SET);
    disk -> directory.structure[entrada_del_directorio][0] = bit_validez;
    char array_de_bytes[1] = {bit_validez};
    fwrite(array_de_bytes, sizeof(char), 1, disk -> file_pointer);
    return entrada_del_directorio;
}

int create_file(Directory directory, unsigned long int identificador_relativo_bloque_indice, char* filename){
    //Bool para verificar si ya existe
    int exists = 0;
    //Guardamos el primer indice en el cual se puede guardar un archivo
    int indice_nuevo_archivo = -1;
    for (int i = 0; i<64; i++){
        //Buscamos si ya existe
        if(is_valid_directory_entry(directory, i)){
            char nombre_aux[28];
            //HAcemos que la variable nombre_aux, obtenga el nombre del archivo
            nombre_archivo(directory, i, nombre_aux);
            if(strcmp(filename, nombre_aux) == 0){
                printf("Archivo ya existente, intente con otro nombre \n");
                exists = 1;
            }
        }

        //Escribimos en el primer lugar que "no es valido", es decir que esta "borrado" en el directorio
        if (!is_valid_directory_entry(directory, i) && indice_nuevo_archivo == -1){
            indice_nuevo_archivo = i;
        }
    }

    //Si el archivo no existe, y hay espacio, entonces se crea
    if(!exists && indice_nuevo_archivo != -1){
        modify_directory_entry(&directory, identificador_relativo_bloque_indice, indice_nuevo_archivo, filename, 0x01);
        printf("Archivo %s creado con éxito\n", filename);
        return indice_nuevo_archivo;
    }
    return -1;
}

int delete_file(Directory directory, char* filename){
    for (int i = 0; i<64; i++){
        //Buscamos el primer archivo con ese nombre
        if (is_valid_directory_entry(directory, i)){
            char nombre_aux[28];
            //HAcemos que la variable nombre_aux, obtenga el nombre del archivo
            nombre_archivo(directory, i, nombre_aux);
            if(strcmp(filename, nombre_aux) == 0){
                //Marcamos el bit de validez con 0
                modify_entry_valid_byte(directory, i, 0x00);
                printf("Archivo %s borrado con exito", filename);
                return i;
            }

        }
    }
    printf("Archivo inexistente en el disco\n");
    return -1;

}

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