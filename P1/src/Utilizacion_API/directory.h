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
    char structure[64][32];

} Directory;


/*Metodo para inicializar un directorio vacio*/
Directory directory_init();

/*Metodo para verificar si la entrada es valida o no (primer byte de una entrada)*/
int is_valid_directory_entry(Directory directory, int index);

/*Metodo para modificar la entrada*/
int modify_directory_entry(Directory* directory, int index, char* filename, char bit_validez);

/*Metodo para representar una entrada*/
int print_entry(Directory directory, int index);

/*Metodo para imprimir solamente el nombre de archivo en una entrada*/
int nombre_archivo(Directory directory, int index, char nombre[28]);

/**/