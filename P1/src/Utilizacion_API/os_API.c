#include "directory.h"
#include "os_API.h"
#include "bloqueindice.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void os_mount(char *diskname, int partition_id)
{
  /* Toma las variables globales declaradas en el header y les asigna valor */
  int indice;
  disk = init_disk(diskname);

  indice = get_partition_index(partition_id);
  particion_montada = disk->mbt->entradas[indice];
  disk->directory = directory_init(indice);
}

Disk *init_disk(char *filename)
{
  printf("FILENAME: %s\n", filename);
  FILE *fp = fopen(filename, "r+b");
  Disk *disk = malloc(sizeof(Disk));
  *disk = (Disk){
      .file_pointer = fp,
      .name = filename,
      .mbt = init_mbt(fp),
  };
  if (disk->file_pointer == NULL)
  {
    // Si no existe el archivo
    printf("Filename doesnt exist\n");
    return NULL;
  }
  else
  {
    return disk;
  }
}

Mbt *init_mbt(FILE *fp)
{
  unsigned char bytes_leidos[8];
  Mbt *mbt = calloc(1, sizeof(Mbt));
  fseek(fp, 0, SEEK_SET);
  for (int i = 0; i < 128; i++)
  {
    fseek(fp, i * 8, SEEK_SET);
    fread(bytes_leidos, sizeof(char), 8, fp);
    // printf("Bytes leidos: %d %d %d %d %d %d %d %d \n", bytes_leidos[0], bytes_leidos[1], bytes_leidos[2], bytes_leidos[3], bytes_leidos[4], bytes_leidos[5], bytes_leidos[6], bytes_leidos[7]);
    for (int j = 0; j < 8; j++)
    {
      // printf("i: %i, j: %i \n", i, j);
      // printf("byte leido en 0: %d \n", bytes_leidos[0]);
      mbt->entradas[i][j] = bytes_leidos[j];
    }
  }
  return mbt;
}

// MBT functions
int is_partition_valid(int indice)
{
  int bit;
  unsigned char primer_byte;

  primer_byte = disk->mbt->entradas[indice][0];
  bit = primer_byte & (1 << 7) ? 1 : 0;

  return bit;
}

int get_partition_index(int id)
{
  for (int i = 0; i < 128; i++)
  {
    if (get_partition_id(i) == id)
    {
      return i;
    }
  }
  return -1;
}

int get_partition_block_id(int indice)
{
  unsigned char *entrada = disk->mbt->entradas[indice];
  unsigned long int abs_block_id;

  abs_block_id = (entrada[1] << 16) | (entrada[2] << 8) | (entrada[3]);

  return abs_block_id;
}

// Se obtiene ID_unico de particion a partir de índice en estructura Mbt
int get_partition_id(int indice)
{
  unsigned char *entrada = disk->mbt->entradas[indice];
  unsigned int partition_id;

  partition_id = entrada[0] & ((1 << 7) - 1);

  return partition_id;
}

int get_partition_size(int indice)
{
  unsigned char *entrada = disk->mbt->entradas[indice];
  unsigned long int abs_block_id;
  unsigned long int partition_size;
  unsigned int partition_id;
  int partition_valid;

  partition_valid = entrada[0] & (1 << 7) ? 1 : 0;
  partition_id = entrada[0] & ((1 << 7) - 1);
  abs_block_id = (entrada[1] << 16) | (entrada[2] << 8) | (entrada[3]);
  partition_size = (entrada[4] << 24) | (entrada[5] << 16) | (entrada[6] << 8) | (entrada[7]);

  printf("[Partition id: %i], VALID?: %i, partition_size: %li, ID_primer_bloque: %li \n", partition_id, partition_valid, partition_size, abs_block_id);

  return partition_size;
}

int os_mbt()
{

  for (int i = 0; i < 128; i++)
  {
    if (is_partition_valid(i))
    {
      printf("Particion con ID: %i valida. \n", get_partition_id(i));
    }
  }
  return 0;
}

int os_delete_partition(int id)
{
  int identificador_relativo;
  int primer_byte_entrada;
  if (id > 127)
  {
    // out of range
    return 1;
  }
  primer_byte_entrada = disk->mbt->entradas[id][0];
  identificador_relativo = primer_byte_entrada & ((1 << 7) - 1);
  // printf("Entrada es: %d\n", primer_byte_entrada);
  // printf("Identificador es: %i\n", identificador);
  // Cambiamos el primer bit del primer byte de la entrada a 0
  primer_byte_entrada = (primer_byte_entrada & ~(1UL << 8)) | (0 << 8);

  return 0;
}

int os_reset_mbt()
{
  for (int i = 0; i < 128; i++)
  {
    os_delete_partition(i);
  }
  return 0;
}

// int os_create_partition(int id, int size){
//     int first_invalid_entry;
//     int next_valid_entry;
//     int inicio_invalid_entry;

//     int ocupacion_disco[128][2];

//     if (is_partition_valid(id)){
//         // Partición con ese id ya está tomada
//         return 1;
//     }
//     for (int i = 0; i < 128; i++){
//             if (is_partition_valid(i)){
//                 ocupacion_disco[i][0] = get_partition_block_id(i);
//                 ocupacion_disco[i][1] = get_partition_block_id(i) + get_partition_size(i);
//             }

//     for (int i = 0; i < 128; i++){
//         if (!is_partition_valid(i)){
//             first_invalid_entry = i;
//             next_valid_entry = get_next_valid_entry(i);
//             inicio_invalid_entry = 0;

//             if (next_valid_entry != -1){
//                 // Chequear si cabe
//                 break;

//             }
//         }
//     }

//     return 0;
//     }
// }

int get_next_valid_entry(int index)
{
  for (int i = index; i < 128; i++)
  {
    if (is_partition_valid(i))
    {
      return i;
    }
  }
  return -1;
}

int get_previous_valid_entry(int index)
{
  for (int i = index; i > -1; i--)
  {
    if (is_partition_valid(i))
    {
      return i;
    }
  }
  return -1;
}

int os_exists(char *filename)
{
  /*Cabe recalcar que se busca solo en la partición actual*/
  /*Esta funcion asume que la estructura de directorio ya está creada en la particion*/
  /*Issue N°208: El unico bloque tipo directorio es el raiz en una particion*/

  for (int i = 0; i < 64; i++)
  {

    //VErificamos si la entrada es valida
    if (is_valid_directory_entry(disk->directory, i))
    {
      //Creamos variable en la cual guardamos el nombre
      char nombre_actual[28];
      //Buscamos el nombre
      nombre_archivo(disk->directory, i, nombre_actual);
      //Retorna solo si la encuentra
      if (strcmp(nombre_actual, filename) == 0)
      {
        return 1;
      }
    }
  }

  return 0;
}

void os_ls()
{

  for (int i = 0; i < 64; i++)
  {
    if (is_valid_directory_entry(disk->directory, i))
    {
      char nombre_actual[28];
      //Buscamos el nombre
      nombre_archivo(disk->directory, i, nombre_actual);
      printf("%s\n", nombre_actual);
    }
  }
}

void os_bitmap(unsigned block)
{
  printf("-----------------BITMAP-------------\n");
  //Hexadecimal

  if (block == 0)
  {
    //32 bytes por entrada
    fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048, SEEK_SET); //Primer bloque de bitmap
    unsigned char *value = malloc(sizeof(unsigned char));
    int used = 0;
    int contador = 0;
    for (int i = 0; i < 2048; i++)
    { //Falta multiplicarlo por el numero de bloques
      fread(value, sizeof(unsigned char), 1, disk->file_pointer);
      for (int i = 7; i > -1; i--)
      {
        if (((*value >> i) & 1) == 1)
        {
          used++;
          fprintf(stderr, "1");
          //printf("1");
        }
        else
        {
          //printf("0");
          fprintf(stderr, "0");
        }
        contador++;
      }
    }
    printf("\nBloques LIBRES %d ", (contador - used));
    printf("\nBloques OCUPADOS %d\n", used);
    free(value);
  }
  else
  {
    fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048 + (2048 * (block - 1)), SEEK_SET);
    unsigned char *value = malloc(sizeof(unsigned char));
    int used = 0;
    int contador = 0;
    for (int i = 0; i < 2048; i++)
    {
      fread(value, sizeof(unsigned char), 1, disk->file_pointer);
      for (int i = 7; i > -1; i--)
      {
        if (((*value >> i) & 1) == 1)
        {
          used++;
          fprintf(stderr, "1");
          //printf("1");
        }
        else
        {
          //printf("0");
          fprintf(stderr, "0");
        }
        contador++;
      }
    }
    printf("\nBloques LIBRES %d \n", (contador - used));
    printf("\nBloques OCUPADOS %d\n", used);
    free(value);
  }
}

int bitmap_update(int block)
{
  fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048, SEEK_SET); //Ponemos el puntero despúes del bloque de directorio 205824=1024+ 2*1024*50 +2048
  int contador = 0;
  unsigned char *value = malloc(sizeof(unsigned char));
  for (int i = 0; i < 2048; i++)
  { //2048 Cantidad de entradas bitmap Falta multiplicarlo por le numero de bloques que tiene el bitmap
    fread(value, sizeof(unsigned char), 1, disk->file_pointer);
    for (int j = 7; j > -1; j--)
    {
      if (contador == block)
      {
        fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048 + i, SEEK_SET);
        *value += 1 << j; //Para escribir el bit
        fwrite(value, sizeof(unsigned char), 1, disk->file_pointer);
        free(value);
        return 1;
      }
      contador++;
    }
  }
  return 0;
}

int available_block()
{
  //Numero de bytes
  int bytes = 128 * 1024;                                                           //128 blques
  fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048, SEEK_SET); //Vamos al bitmap
  unsigned char *value = malloc(sizeof(unsigned char));
  int block = 0; //El bloque que vamos a retornar
  for (int i = 0; i < bytes; i++)
  {
    fread(value, sizeof(unsigned char), 1, disk->file_pointer);
    for (int i = 7; i > -1; i--)
    {
      if (((*value >> i) & 1) == 0)
      {
        free(value);
        return (unsigned int)block;
      }
      block++;
    }
  }
  free(value);
  return 0; //Si no hay bloques disponibles
}

// int available_directory(int ptr){
//   fseek(disk -> file_pointer, ptr, SEEK_SET);
//   unsigned char * buffer = malloc (sizeof(unsigned char)*32);
//   for (int i = 0; i < 31; i++){
//       fseek(disk -> file_pointer, ptr +(i*32), SEEK_SET);
//       fread(buffer,sizeof(unsigned char),32,disk -> file_pointer);
//       if (buffer[0] == (unsigned char)2 || buffer[0] == (unsigned char)4 || buffer[0] == (unsigned char)8 || buffer[0] == (unsigned char)16 || buffer[0] == (unsigned char)32){
//         continue;
//       }
//       else{
//         free(buffer);
//         return ptr +(i*32);
//       }
//   }
//   free(buffer);
//   return 0;
// }

unsigned int file_data(unsigned int pt)
{
  unsigned char *buffer = malloc(sizeof(unsigned char) * 5);
  fseek(disk->file_pointer, pt, SEEK_SET);
  fread(buffer, sizeof(unsigned char), 5, disk->file_pointer);
  // printf("buffer\n");
  // for (int i=0 ; i<5;i++){
  //   printf("%x\n",buffer[i]);
  // }
  unsigned int size = buffer[0] << 32 | buffer[1] << 24 | buffer[2] << 16 | buffer[3] << 8 | buffer[4];
  free(buffer);
  return size;
};
// se espera que el buffer venga creado desde antes (array vacio) del tamano de n_bytes
//unsigned char *buffer = malloc(sizeof(unsigned char) * n_bytes_a_leer );

int os_read(osFILE *file_desc, void *buffer, int nbytes)
{

  int n_bytes_a_leer;
  // si n_bytes es menor a lo que queda por leer del archivo lee lo que queda
  if (nbytes >= file_desc->size - file_desc->bytes_read)
  {
    n_bytes_a_leer = file_desc->size - file_desc->bytes_read;
  }
  // si n_bytes es mayor a lo que queda por leer del archivo lee n_bytes
  else
  {
    n_bytes_a_leer = nbytes;
  }
  fseek(disk->file_pointer, file_desc->index_ptr + 5, SEEK_SET);
  // obtener puntero absoulto de comienzo de la particion
  int puntero_abs_particion = disk->directory.directory_byte_pos;
  // indice del bloque de incio de la lectura
  int bloque_inicio_lectura = floor(file_desc->bytes_read / 2048);
  int offset_bloque = file_desc->bytes_read - bloque_inicio_lectura * 2048;
  int i = 0;
  int primer_bloque = 1;
  //nos posicionamos en el bloque indice donde debemos empezar a leer

  while (i < n_bytes_a_leer)
  {
    fseek(disk->file_pointer, file_desc->index_ptr + 5 + bloque_inicio_lectura * 3, SEEK_SET);
    char posicion_relativa[3];
    fread(posicion_relativa, sizeof(unsigned char), 3, disk->file_pointer);
    unsigned long int pos_relativa_bloque_datos = (posicion_relativa[0] << 16) | (posicion_relativa[1] << 8) | posicion_relativa[2];
    //llegamos al puntero de bloque de datos
    if (primer_bloque)
    {
      fseek(disk->file_pointer, puntero_abs_particion + pos_relativa_bloque_datos * 2048 + offset_bloque, SEEK_SET);
      printf("ubicacion bloque datos: %i %lu\n", bloque_inicio_lectura, puntero_abs_particion + pos_relativa_bloque_datos * 2048 + offset_bloque);
      fread(buffer + i, sizeof(unsigned char), 2048 - offset_bloque, disk->file_pointer);
      primer_bloque = 0;
      i += (2048 - offset_bloque);
    }
    else if (n_bytes_a_leer - i <= 2048)
    {
      fseek(disk->file_pointer, puntero_abs_particion + pos_relativa_bloque_datos * 2048, SEEK_SET);
      printf("ubicacion bloque datos: %i %lu\n", bloque_inicio_lectura, puntero_abs_particion + pos_relativa_bloque_datos * 2048);
      fread(buffer + i, sizeof(unsigned char), n_bytes_a_leer - i, disk->file_pointer);
      i += n_bytes_a_leer - i;
    }

    else
    {
      fseek(disk->file_pointer, puntero_abs_particion + pos_relativa_bloque_datos * 2048, SEEK_SET);
      printf("ubicacion bloque datos: %i %lu\n", bloque_inicio_lectura, puntero_abs_particion + pos_relativa_bloque_datos * 2048);
      fread(buffer + i, sizeof(unsigned char), 2048, disk->file_pointer);
      i += 2048;
    }
    bloque_inicio_lectura += 1;
  }
  //fprintf(stderr, "0");
  file_desc->bytes_read += n_bytes_a_leer;
  return n_bytes_a_leer;
}
int bitmap_invalid(int block)
{
  fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048, SEEK_SET); //Primer bloque de bitmap, 128 bloque siguientesal bloque de directorio
  int contador = 0;
  unsigned char *value = malloc(sizeof(unsigned char));
  for (int i = 0; i < 128 * 1024; i++)
  {
    fread(value, sizeof(unsigned char), 1, disk->file_pointer);
    for (int j = 7; j > -1; j--)
    {
      if (contador == block)
      {
        fseek(disk->file_pointer, (disk->directory.directory_byte_pos) + 2048 + i, SEEK_SET);
        if (*value & (1 << j) != 0)
        {
          *value -= 1 << j; //Para escribir el bit
          fwrite(value, sizeof(unsigned char), 1, disk->file_pointer);
        }
        free(value);
        return 1;
      }
      contador++;
    }
  }
  free(value);
  return 0;
}

int os_rm(char *filename)
{
  if (os_exists(filename))
  {
    int pos_absoluta_bloque_indice = get_file_index_absolute_ptr(disk->directory, filename);
    fseek(disk->file_pointer, pos_absoluta_bloque_indice + 5, SEEK_SET);
    // marcamos validos los bloques de datos del bitmap
    for (int i = 0; i < 681; i++)
    {
      char posicion_relativa[3];
      fread(posicion_relativa, sizeof(unsigned char), 3, disk->file_pointer);
      unsigned long int pos_relativa_bloque_datos = (posicion_relativa[0] << 16) | (posicion_relativa[1] << 8) | posicion_relativa[2];
      bitmap_invalid(pos_relativa_bloque_datos);
    }
    // ahora liberamos bloque indice
    int pos_relativa_bloque_indice = get_file_index_relative_ptr(disk->directory, filename);
    bitmap_invalid(pos_relativa_bloque_indice);
    // liberamos la entrada del directorio
    delete_file(disk->directory, filename);
    return 0;
  }
  printf("archivo inexistente\n");
  return 1;
}

osFILE *os_open(char *filename, char mode)
{
  osFILE *os_file = malloc(sizeof(osFILE));
  strcpy(os_file->name, filename);
  if (mode == 'r')
  {
    int existe = os_exists(filename);
    if (existe)
    {
      int dir_block = get_directory_id_by_name(disk->directory, filename);
      int index_block = get_index_relative_position(disk->directory, dir_block);
      os_file->directory_ptr = disk->directory.directory_byte_pos + dir_block * 32;
      os_file->index_ptr = disk->directory.directory_byte_pos + index_block * 2048;
      os_file->size = file_data(os_file->index_ptr);
      os_file->bytes_read = 1; //Se inicia en 1
      return os_file;
    }
    else
    {
      printf("ARCHIVO NO EXISTE\n");
    }
  }
  else if (mode == 'w')
  {
    int existe = os_exists(filename);
    if (existe)
    {
      printf("ARCHIVO YA EXISTE\n");
      return 0;
    }
    else
    {
      int block = available_block();
      if (block == 0)
      {
        printf("NO HAY BLOQUES DISPONIBLES\n");
        return 0;
      }
      else
      {

        os_file->index_ptr = disk->directory.directory_byte_pos + block * 2048;
        bitmap_update(block);
        //int ptr = available_directory(os_file->directory_ptr);
        //7. Crear el archivo
        os_file->size = 0;
        escribir_bloque_indice(os_file->index_ptr, os_file->size, 1024);
        //4. Buscar una entrada en el directorio
        int ptr = create_file(disk->directory, block, os_file->name);
        if (ptr == -1)
        {
          printf("NO HAY BLOQUES DE DIRECTORIO DISPONIBLES\n");
          return 0;
        }
        else
        {
          os_file->directory_ptr = disk->directory.directory_byte_pos + ptr * 32;
        }
      }

      return os_file;
    }
  }
}

/*
return contador: bytes escritos efectivamente
*/
int os_write(osFILE *file_desc, void *buffer, int nbytes)
{
  /*Si nbytes supera el tamaño maximo, solo escribo el maximo*/
  int max_size = 2048 * 681;
  if (nbytes > max_size)
    nbytes = max_size;

  /*Escribir los nbytes*/
  int contador = 0;
  int bloques = 0;
  while (contador < nbytes)
  {
    /*Encontrar un bloque libre*/
    int bloque_libre = available_block();
    if (bloque_libre == 0)
    {
      /*No hay bloques libres, detener escritura*/
      printf("[WRITE] No quedan bloques libres");
      /*Retornar bytes totales escritos*/
      file_desc->size = contador;
      fseek(disk->file_pointer, file_desc->index_ptr, SEEK_SET);
      fwrite(&contador, sizeof(char), 5, disk->file_pointer);
      return contador;
    }
    else
    {
      printf("Tratando de escribir\n");
      /*Se puede escribir*/
      int puntero_libre = 0;
      int bytes_a_escribir = 0;
      puntero_libre = disk->directory.directory_byte_pos + bloque_libre * 2048;
      printf("Puntero: %d\n", puntero_libre);
      bytes_a_escribir = nbytes - contador;
      if (bytes_a_escribir > 2048)
        bytes_a_escribir = 2048;
      printf("Escribir: %d\n", bytes_a_escribir);
      fseek(disk->file_pointer, puntero_libre, SEEK_SET);
      int actual = fwrite(&buffer, sizeof(char), bytes_a_escribir, disk->file_pointer);
      /*Actualizr bytes escritos*/
      contador += actual;
      /*Actualizar bitmap*/
      bitmap_update(bloque_libre);
      /*Actualizar indice*/
      int puntero_bloque = 0;
      /*Verificar esta parte*/
      puntero_bloque = file_desc->index_ptr + 5 + bloques * 3;
      printf("Bloque numero %d\n", puntero_bloque);
      fseek(disk->file_pointer, bloque_libre, SEEK_SET);
      fwrite(&puntero_bloque, sizeof(char), 3, disk->file_pointer);
      bloques += 1;
    }
  }
  /*Retornar bytes totales escritos*/
  file_desc->size = contador;
  fseek(disk->file_pointer, file_desc->index_ptr, SEEK_SET);
  fwrite(&contador, sizeof(char), 5, disk->file_pointer);
  printf("[WRITE] Bytes escritos %d\n", contador);
  printf("[WRITE] Bloques usados %d\n", bloques);
  return contador;
}