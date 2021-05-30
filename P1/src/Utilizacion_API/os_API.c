#include "directory.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>


void os_mount(char* diskname, int partition){
    /* Toma las variables globales declaradas en el header y les asigna valor */
    disk = init_disk(diskname);
    particion_montada = disk -> mbt->entradas[partition];
    
}

Disk* init_disk(char* filename){
    printf("FILENAME: %s\n", filename);
    Disk* disk = malloc(sizeof(Disk));
    *disk = (Disk) {
        .file_pointer = fopen(filename, "r+b"),
        .name = filename,
        .mbt = init_mbt()
    };
    if (disk -> file_pointer == NULL){
        // esta mal 
        printf("Filename doesnt exist\n");
        return NULL;
    }
    else
    {
        return disk;
    }

}

Mbt* init_mbt()
{
    Mbt* mbt = malloc(sizeof(Mbt));
    for (int i = 0; i < 128; i++){
        mbt->entradas[i][0] = 0b00000000;
    }
    return mbt;
}

int is_partition_valid(Mbt* mbt, int index){
    int bit;
    int number;
    number = mbt->entradas[index][0];
    for (int i = 0; i < 8; i++){
        bit = (number >> i) & 1U;
        printf("Bit at position %i of partition index: %i is: %i \n", i, index, bit);
    }
    return bit;
}

int os_delete_partition(Mbt* mbt, int id){
    int identificador;
    int primer_byte_entrada;
    if (id > 127){
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

void os_reset_mbt(Mbt* mbt, int id){
    int identificador;
    int entrada;
    for (int i = 0; i < 128; i++){
       os_delete_partition(mbt, i);
    }
}

int os_exists(char* filename){
    /*Cabe recalcar que se busca solo en la partición actual*/
    /*Esta funcion asume que la estructura de directorio ya está creada en la particion*/
    /*Issue N°208: El unico bloque tipo directorio es el raiz en una particion*/
    
    //Supongo que directorio sera una variable global
    Directory prueba = directory_init();
    //Creo un directorio simulado por ahora 
    modify_directory_entry(&prueba, 4,filename, 0b00000001);

    for (int i = 0; i<64; i++){
        //Creamos variable en la cual guardamos el nombre
        char nombre_actual[28];
        //Buscamos el nombre
        nombre_archivo(prueba, i, nombre_actual);
        //Retorna solo si la encuentra
        if (strcmp(nombre_actual, filename) == 0){
            return 1;
        }
    }

    return 0;

}

void os_ls(){
    //Supongo que directorio sera una variable global
    Directory prueba = directory_init();
    //Creo muchos directorios para simular por ahora 
    modify_directory_entry(&prueba, 0,"1.png", 0b00000001);
    modify_directory_entry(&prueba, 1,"2.png", 0b00000001);
    modify_directory_entry(&prueba, 2,"3.png", 0b00000001);
    modify_directory_entry(&prueba, 3,"4.png", 0b00000001);
    modify_directory_entry(&prueba, 4,"5.png", 0b00000001);
    modify_directory_entry(&prueba, 5,"6.png", 0b00000001);
    modify_directory_entry(&prueba, 6,"Tesis.docx", 0b00000001);

    for (int i =0; i<64; i++){
        if (is_valid_directory_entry(prueba, i)){
            char nombre_actual[28];
            //Buscamos el nombre
            nombre_archivo(prueba, i, nombre_actual);
            printf("%s\n", nombre_actual);
        }

    }

}

void os_bitmap(unsigned block){
    printf("-----------------BITMAP-------------\n");
  //Hexadecimal
  
    if (block == 0){
      //32 bytes por entrada
      fseek(disk -> file_pointer, 105472, SEEK_SET); //Primer bloque de bitmap
      unsigned char * value = malloc(sizeof(unsigned char));
      int used = 0;
      int contador = 0;
      for (int i = 0; i < 2048; i++){  //Falta multiplicarlo por el numero de bloques
        fread(value, sizeof(unsigned char), 1, disk -> file_pointer);
        for (int i = 7; i > -1; i--){
          if(((*value >> i) & 1) == 1){
            used ++;
            fprintf(stderr, "1");
            //printf("1");
          }
          else{
            //printf("0");
            fprintf(stderr, "0");
          }
          contador++;
        }
      }
      printf("\nBloques LIBRES %d ",(contador-used));
      printf("\nBloques OCUPADOS %d\n",used);
      free(value);
    }
    else{
      fseek(disk ->file_pointer, 105472 + (2048*(block-1)), SEEK_SET);
      unsigned char * value = malloc(sizeof(unsigned char));
      int used = 0;
      int contador = 0;
      for (int i = 0; i < 2048; i++){
        fread(value, sizeof(unsigned char), 1, disk ->file_pointer);
        for (int i = 7; i > -1; i--){
          if(((*value >> i) & 1) == 1){
            used ++;
            fprintf(stderr, "1");
            //printf("1");
          }
          else{
            //printf("0");
            fprintf(stderr, "0");
          }
          contador++;
        }
      }
      printf("\nBloques LIBRES %d \n",(contador-used));
      printf("\nBloques OCUPADOS %d\n",used);
      free(value);

    }

  


}


int bitmap_update(int block){
  fseek(disk -> file_pointer, 105472, SEEK_SET); //Ponemos el puntero despúes del bloque de directorio 205824=1024+ 2*1024*50 +2048
  int contador = 0;
  unsigned char * value = malloc(sizeof(unsigned char));
  for (int i = 0; i < 2048; i++){             //2048 Cantidad de entradas bitmap Falta multiplicarlo por le numero de bloques que tiene el bitmap
    fread(value, sizeof(unsigned char), 1, disk -> file_pointer);
    for (int j = 7; j > -1; j--){
      if (contador == block){
        fseek(disk -> file_pointer, 105472+i, SEEK_SET);
        *value += 1 << j; //Para escribir el bit
        fwrite(value, sizeof(unsigned char), 1, disk -> file_pointer);
        free(value);
        return 1;
      }
      contador ++;
    }
  }
  return 0;
}