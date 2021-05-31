// Tells the compiler to compile this file once
#pragma once


/*Representacion de Directorio*/
/*
Este bloque se separa en entradas de 32B
donde cada una representa un archivo.

Este bloque esta compuesto por una secuencia de entradas de directorio, donde cada entrada
de directorio ocupa 32 Bytes, por lo tanto, puede haber como maximo 64 entradas en un bloque de directorio
*/

typedef struct directory {
    //Estructura de 64 entradas de 32 bit. EL identificador relativo se guarda en las entradas.
    unsigned char structure[64][32];
    //Numero de bloque absoluto en el disco del directorio en la particion actual.
    unsigned long int directory_block_pos;
    //Numero de bytes absoluto en el disco para llegar al bloque directorio de la particion actual.
    unsigned long int directory_byte_pos;


} Directory;


/*Metodo para inicializar un directorio vacio*/
Directory directory_init(int ubicacion_bytes_particion);

/*Metodo para verificar si la entrada es valida o no (primer byte de una entrada)*/
int is_valid_directory_entry(Directory directory, int index);

/*Metodo para modificar la entrada, es llamado solo por la funcion create_file*/
int modify_directory_entry(Directory* directory,unsigned long int identificador_relativo_bloque_indice, unsigned long int entrada_del_directorio, char* filename, char bit_validez);

/*Metodo para modificar solo el bit de validez*/
int modify_entry_valid_byte(Directory directory,unsigned long int entrada_del_directorio, char bit_validez);

/*MEtodo para "Guardar" archivos en la estructura del directorio en el primer indice que enuentra y retorna -1 en caso de error*/
int create_file(Directory directory, unsigned long int identificador_relativo_bloque_indice, char* filename);

/*Metodo para "borrar" Archivos en la estructura del directorio, marcando el bit de validez con un 0*/
int delete_file(Directory directory, char* filename);


/*Metodo para representar una entrada*/
int print_entry(Directory directory, int index);

/*Metodo para imprimir solamente el nombre de archivo en una entrada, no verifica si existe o no*/
int nombre_archivo(Directory directory, int index, char nombre[28]);

/*Metodo para obtener bloque relativo en la particion de algun archivo, retorna -1 en caso de que no sea valido*/
int get_index_relative_position(Directory directory, int index);

/*Metodo para obtener bloque relativo en el directorio de algun archivo, retorna -1 en caso de que no sea valido*/
int get_directory_id_by_name(Directory directory, char* filename);