#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
#include <pthread.h>

typedef struct conect_info{
	int server_socket;
  int solicitud;
}Conect_info;


void* get_input(void *arg){
  Conect_info *info = (Conect_info *)arg;
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  client_send_message(info->server_socket,info->solicitud, response);
  free(info);
  
}

void manejo_caso(int server_socket, int solicitud){
  char * message = client_receive_payload(server_socket);
  printf("%s\n", message);
  free(message);
  Conect_info *info = (Conect_info *)malloc(sizeof(Conect_info));
  info->server_socket = server_socket;
  info->solicitud = solicitud;
  pthread_t id;
  pthread_create(&id,NULL,(void *)get_input,(void *)info);
  pthread_detach(id);
}

int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  
  //Valores default
  char * IP = "0.0.0.0";
  int PORT = 8080;
  //Manejo de flags
  int defined_ip = 0;
  int defined_port = 0;
  for (int i = 0; i < argc; i++){
      //Flag de direccion ip
      if (strcmp("-i", argv[i]) == 0 && i + 1 < argc){
          IP = argv[i + 1];
          defined_ip = 1;
      }
      if(strcmp("-p", argv[i]) == 0 && i + 1 < argc){
          PORT = atoi(argv[i + 1]);
          defined_port = 1;
      }
  }
  if (!defined_ip || !defined_port){
      printf ("Para correr, debe llamar de la siguiente manera:\n>./client -i <ip_address> -p <tcp_port>\n\n");
      return 0;
  }



  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);
    switch (msg_code){

    // Recive bienvenida al juego envia nombre jugador
    case 0:
      manejo_caso(server_socket, 0);
    break;

    // Recive solicitud de clase envia clase
    case 1:
      manejo_caso(server_socket, 1);
    break;

    // Recive solicitud de inicio de juego, envia inicio de juego
    case 2:
      manejo_caso(server_socket, 2);
    break;

    // Recive solicitud de eleccion de monstruo, envia tipo
    case 3:
      manejo_caso(server_socket, 3);
    break;

    // caso turno jugador
    case 4:
      manejo_caso(server_socket, 4);
    break;

    // Solo recive informacion
    case 5:{
      char * message = client_receive_payload(server_socket);
      printf("%s", message);
      free(message);
      }
    break;
    
    //Caso finalizacion del juego, Server pregunta si  quiere seguir jugando
    case 6:
      manejo_caso(server_socket, 6);
    break;

    case 7:{//desconectar al jugador
      close(server_socket);
      pthread_exit(NULL);
    break;
    }
   }
 }
  // Se cierra el socket
  close(server_socket);
  free(IP);
  return 0;
}