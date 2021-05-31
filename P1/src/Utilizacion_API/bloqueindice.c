#include <stdio.h>  // FILE, fopen, fclose, etc.
#include"bloqueindice.h"
#include"../Utilizacion_API/os_API.h"
// el puntero absouluto es la posicion en el disco donde queremos poner nuestro bloque indice,inicio es el puntero relativo donde comienzan los bloques
void escribir_bloque_indice(int puntero_absoluto, unsigned long int file_size, unsigned long int inicio){

    // escribimos el file_size
    unsigned char bytes[5]; 
    bytes[0] = file_size >> 32 & 0xFF;
    bytes[1] = file_size >> 24 & 0xFF;
    bytes[2] = file_size >> 16 & 0xFF;
    bytes[3] = file_size >> 8  & 0xFF;
    bytes[4] = file_size       & 0xFF;
    fseek(disk -> file_pointer,puntero_absoluto , SEEK_SET);
    fwrite(bytes, sizeof(char), 5, disk -> file_pointer);
    // escribimos los punteros relativos a los bloques
    for(int i =0; i<681; i++){
        unsigned int direccion = inicio + i;
        unsigned char puntero_bloque[3]; 
        puntero_bloque[0] = direccion >> 16 & 0xFF;
        puntero_bloque[1] = direccion >> 8  & 0xFF;
        puntero_bloque[2] = direccion       & 0xFF;
        fwrite(puntero_bloque, sizeof(char), 3, disk -> file_pointer);
    }
}
