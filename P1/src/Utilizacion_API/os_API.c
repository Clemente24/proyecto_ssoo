#include "directory.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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


int available_block(){
  //Numero de bytes
  int bytes = 128*1024; //128 blques
  fseek(disk -> file_pointer, 105472, SEEK_SET); //Vamos al bitmap
  unsigned char * value = malloc(sizeof(unsigned char));
  int block = 0; //El bloque que vamos a retornar
  for (int i = 0; i < bytes; i++){
    fread(value, sizeof(unsigned char), 1, disk -> file_pointer);
    for (int i = 7; i > -1; i--){
      if(((*value >> i) & 1) == 0){
        free(value);
        return (unsigned int)block;
      }
      block ++;
    }
  }
  free(value);
  return 0; //Si no hay bloques disponibles
}



int available_directory(int ptr){
  fseek(disk -> file_pointer, ptr, SEEK_SET);
  unsigned char * buffer = malloc (sizeof(unsigned char)*32);
  for (int i = 0; i < 31; i++){
      fseek(disk -> file_pointer, ptr +(i*32), SEEK_SET);
      fread(buffer,sizeof(unsigned char),32,disk -> file_pointer);
      if (buffer[0] == (unsigned char)2 || buffer[0] == (unsigned char)4 || buffer[0] == (unsigned char)8 || buffer[0] == (unsigned char)16 || buffer[0] == (unsigned char)32){
        continue;
      }
      else{
        free(buffer);
        return ptr +(i*32);
      }
  }
  free(buffer);
  return 0;
}


unsigned int file_data(unsigned int pt){
  unsigned char *buffer = malloc(sizeof(unsigned char) * 4);
  fseek(disk ->file_pointer, pt, SEEK_SET);
  fread(buffer, sizeof(unsigned char), 4, disk ->file_pointer);
  unsigned int size = (unsigned int)buffer[0] * pow(16,6)+
                      (unsigned int)buffer[1] * pow(16,4) +
                      (unsigned int)buffer[2] * pow(16,2) +
                      (unsigned int)buffer[3];
  free(buffer);
  return size;
};


osFILE* os_open(char* filename, char mode){
    osFILE *os_file = malloc(sizeof(osFILE));
    os_file -> name =filename
    if (mode == 'r'){
        int existe = os_exists(filename);
        if(existe){
        r= true;
      os_file->directory_ptr = ftell(disk -> file_pointer);
      os_file->index_ptr =  ftell(disk -> file_pointer);
      os_file->size = file_data(os_file->index_ptr);
      
      return os_file;
        } else{
            printf("ARCHIVO NO EXISTE\n");
        }
    } else if (mode =='w'){
        int existe = os_exists(filename);
        if(existe){
             printf("ARCHIVO YA EXISTE\n");
        } else {
        os_file->directory_ptr = ftell(disk -> file_pointer);
        int block = available_block();
        if(block ==0){
            printf("NO HAY BLOQUES DISPONIBLES\n");
        return NULL;
        }else{
            unsigned char * buffer = malloc(sizeof(unsigned char)*32);
            //1. Que la entrada sea de archivo
            buffer[0] = (unsigned char)4;
            //2. Nombre del archivo
            memcpy(buffer + 1, os_file->name, 28);
            //3. Bloque al que pertenece
            unsigned int block_number = htonl(block);
            memcpy(buffer+28, &block_number, 4);
            //4. Buscar una entrada en el directorio
            printf("DIRECTORIO  %d\n", os_file->directory_ptr);
            int ptr = available_directory(os_file->directory_ptr);
            if (ptr == 0){
          int dir_pointer = os_file->directory_ptr;
          while(ptr == 0){
            //Primero reviso si tiene bloque 
            unsigned char * buffer2 = malloc(sizeof(unsigned char)*32);
            fseek(disk -> file_pointer,  dir_pointer + (32*31), SEEK_SET);
            fread(buffer2, sizeof(unsigned char), 32, disk -> file_pointer);
            if (buffer2[0] == (unsigned char)32){
              dir_pointer = ((unsigned int)buffer2[29] * 65536  + (unsigned int)buffer2[30] *256+ (unsigned int)buffer2[31]) * 2048;
              ptr = available_directory(dir_pointer);
              printf("DIRECTORIO TENIA BLOQUE CONTINUE %d BLOQUE DISPONIBLE %d\n", dir_pointer, 0);
              free(buffer2);

            }
            //Si no tiene bloque  lo creo
            else {
              //Debo encontrar un nuevo bloque
              int block2 = available_block();
              if (block2 != 0){
                printf("LINKEANDO BLOQUE \n");
                unsigned int block_number2 = htonl(block2);
                memcpy(buffer2+28, &block_number2, 4);
                buffer2[0] = (unsigned char)32;
                fseek(disk -> file_pointer, os_file->directory_ptr + (32*31), SEEK_SET);
                int escritura = fwrite(buffer2, sizeof(unsigned char), 32, disk -> file_pointer);
                free(buffer2);
                if (escritura != 32){
                  printf("NO SE ESCRIBIERON TODOS LOS BYTES\n");
                  return 0;
                }
                //Creamos un nuevo bloque de directorio
                directory_block(block2, block2*2048, true);
                bitmap_update(block2);
                ptr = available_directory(block2*2048);
                free(buffer2);
              }
              else{
                printf("NO SE PUDO CREAR BLOQUE CONTINUE\n");
                free(buffer2);
                return NULL;
              }
            
            }
          }
        }

        //5. Escribir en el directorio 
        fseek(disk -> file_pointer, ptr, SEEK_SET);
        int escritura = fwrite(buffer, sizeof(unsigned char), 32, disk -> file_pointer);
        if (escritura != 32){
          printf("NO SE ESCRIBIERON TODOS LOS BYTES\n");
          return NULL;
        }

        //6. Actualizar bitmap
        block = available_block();
        os_file -> index_ptr = block*2048;
        bitmap_update(block);

        //7. Crear el archivo
        os_file -> size = 0;
        //index_block(block);
        free(buffer);
        return os_file;
      }
        }

    }
}