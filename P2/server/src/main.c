#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "comunication.h"
#include "conection.h"
#include <time.h>
#include <strings.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int inicio_juego = 0;
int turno = 0;
int jugadores = 0;
int jugadores_activos =0;
int sockets_array[4];

typedef struct sock_info{
	int cfd;
	struct sockaddr_in clie_addr;
	int num;
  int lider;
  int socket_lider;
}s_info;

typedef struct jugador{
  char* nombre;
  char* clase; 
  int lider;
  int vida;
  int vida_maxima;
  int activo;
}Jugador;

typedef struct monstruo{
  char* tipo; 
  int vida;
  int vida_maxima;
}Monstruo;

Jugador lista_jugadores[4];
Monstruo monstruo;

int validar_respuesta(int n_opciones, int opciones[], char * client_message){
  int respuesta = *client_message - '0';
  
  for (int i=0;i<n_opciones;i++){
    if (opciones[i] == respuesta){
        return 1;
    }
  }
  return 0;
}
void seleccionar_clase(char* client_message,int indice ){ 
  int clase = *client_message - '0';
  if(clase ==0){
    lista_jugadores[indice].clase = "Cazador";
    lista_jugadores[indice].vida = 5000;
    lista_jugadores[indice].vida_maxima = 5000;
  }
  
  else if (clase == 1){
    lista_jugadores[indice].clase = "Medico";
    lista_jugadores[indice].vida = 3000;
    lista_jugadores[indice].vida_maxima = 3000;
  }
  
  else{
    lista_jugadores[indice].clase = "Hacker";
    lista_jugadores[indice].vida = 2500;
    lista_jugadores[indice].vida_maxima = 2500;
  }
}

void seleccionar_monstruo(char* client_message){
  int tipo = *client_message - '0';
  //si la seleccion fue aleatoria
  if (tipo == 4){
    //elije numero entre 1 y 3
    tipo = rand() % 4 +1;
  }
  if (tipo == 1){
    monstruo.tipo = "Great JagRuz";
    monstruo.vida = 10000;
    monstruo.vida_maxima = 10000;
  }
  if (tipo == 2){
    monstruo.tipo = "Ruzalos";
    monstruo.vida = 20000;
    monstruo.vida_maxima = 20000;
  }
  if (tipo == 3){
    monstruo.tipo = "Ruiz";
    monstruo.vida = 25000;
    monstruo.vida_maxima = 25000;
  }
}
int turno_jugador(){
  printf("TURNO %i\n",turno % (jugadores));
  return turno % (jugadores);
}

void seleccion_de_poder(int socket){
  Jugador jugador_actual = lista_jugadores[turno_jugador()];
  if (jugador_actual.clase == "Cazador"){
    char * mensaje ="Elije una accion:\n[0]Rendirse\n[1]Estocada\n[2]Corte Cruzado\n[3]Distraer\n";
    server_send_message(socket, 4, mensaje);
  }
  else if(jugador_actual.clase == "Medico"){
    char * mensaje ="Elije una accion:\n[0]Rendirse\n[1]Curar\n[2]Destello Regenerador\n[3]Descarga Vital\n";
    server_send_message(socket, 4, mensaje);

  }
  else{
    char * mensaje ="Elije una accion:\n[0]Rendirse\n[1]Inyeccion SQL\n[2]Ataque DDOS\n[3]Fuerza Bruta\n";
    server_send_message(socket, 4, mensaje);

  }
}
void impresion_estadisticas(){
  
  for (int i=0; i<jugadores;i++){
    char * marco_top = "------------------------ESTADO JUEGO----------------------------\n";
    char * marco_bot = "----------------------------------------------------------------\n";
    server_send_message(sockets_array[i], 5, marco_top);
    char str_turno[100];
    sprintf(str_turno, "Turno numero: %i          Turno de: %s\n", turno, lista_jugadores[turno_jugador()].nombre);
    server_send_message(sockets_array[i], 5, str_turno);
    
    char str_final[100];
    sprintf(str_final, "%s             vida: %i/%i\n",monstruo.tipo,monstruo.vida,monstruo.vida_maxima);
    server_send_message(sockets_array[i], 5, str_final);

    server_send_message(sockets_array[i], 5, marco_bot);

    for(int j=0; j<jugadores;j++){
      char str_final[100];
      sprintf(str_final, "%s[%s]             vida: %i/%i\n",lista_jugadores[j].nombre,lista_jugadores[j].clase,
      lista_jugadores[j].vida,lista_jugadores[j].vida_maxima);
      server_send_message(sockets_array[i], 5, str_final);
    }
    server_send_message(sockets_array[i], 5, marco_bot);
    

  }
  

}

void *thread_cliente(void *arg){
  s_info *s = (s_info *)arg;
  if (s->lider){
    char * mensaje = "Bienvenido Jugador 1 eres el lider del grupo!!\nIngresa tu nombre:";
    server_send_message(s->cfd, 0, mensaje);
  }
  else{
    if (!inicio_juego){
      char * mensaje = "Bienvenido Jugador\nIngresa tu nombre:";
      server_send_message(s->cfd, 0, mensaje);
      }
  
    else
      pthread_exit(NULL);
  }
  
  while (1)
  {
    // Se obtiene el paquete del cliente correspondiente al thread
    int msg_code = server_receive_id(s->cfd);
    switch (msg_code)
    {
    case 0: //recibe nombre jugador envia soliciutd de clase
      {
      char* client_message = server_receive_payload(s->cfd);
      printf("El nombre del jugador es %s\n", client_message);
      lista_jugadores[s->num].nombre = client_message;

      char * mensaje = "Seleccione la clase: \n[1]Cazador\n[2]Medico\n[3]Hacker\n";
      server_send_message(s->cfd, 1, mensaje);
      break;
      }
    
    case 1: //recibe clase jugador, si es lider envia soliciutd de inicio juguego, caso contrario envia mensaje de espera
      {
      char * client_message = server_receive_payload(s->cfd);
      int opciones[3] = {1,2,3};
      if(validar_respuesta(3,opciones,client_message)){
        printf("La clase del jugador es: %s es lider: %i \n", client_message,s->lider);
        seleccionar_clase(client_message,s->num);
        if (s->lider){
          char * mensaje = "Presione enter para iniciar el juego\n";
          server_send_message(s->cfd, 2, mensaje);
          lista_jugadores[s->num].lider = 1;
        }
        else{


          lista_jugadores[s->num].lider = 0;
          char * mensaje = "Personaje listo, a esperar que el lider inicie el juego\n";
          server_send_message(s->cfd, 5, mensaje);
          char str_final[100];
          sprintf(str_final, "El jugador: %s de la clase:%s se ha conectado\n", lista_jugadores[s->num].nombre, lista_jugadores[s->num].clase);
          server_send_message(s->socket_lider, 5, str_final);
        }
      }
      else{
        char * mensaje = "Input no valido\nSeleccione la clase: \n[1]Cazador\n[2]Medico\n[3]Hacker\n";
        server_send_message(s->cfd, 1, mensaje);
      }
      break;}
      
    case 2: //recibe inicio juego envia tipo de monstruo
      {
      char * client_message = server_receive_payload(s->cfd);
      inicio_juego =1;
      char * mensaje = "Seleccione el tipo de monstruo:\n[1]Great JagRuz\n[2]Ruzalos\n[3]Ruiz, el Gemelo Malvado del Profesor Ruz\n[4]Aleatorio\n";
      server_send_message(s->cfd, 3, mensaje);
      break;}
    
    case 3:{ //recibe tipo monstruo envia seleccion de poder 

      char * client_message = server_receive_payload(s->cfd);
      int opciones[4] = {1,2,3,4};
      if(validar_respuesta(4,opciones,client_message)){
        printf("el jugador 0 selecciono el monstruo: %s\n", client_message);
        seleccionar_monstruo(client_message);
        printf("acaaaa");
        impresion_estadisticas();
        seleccion_de_poder(s->cfd);
        turno += 1;
      }
      else{
        char * mensaje = "Input no valido\nSeleccione el tipo de monstruo:\n[1]Great JagRuz\n[2]Ruzalos\n[3]Ruiz, el Gemelo Malvado del Profesor Ruz\n[4]Aleatorio\n";
        server_send_message(s->cfd, 3, mensaje);
      }  
      break;}
    case 4:{ //recibe accion envia un turno nuevo a siguiente jugador

      char * client_message = server_receive_payload(s->cfd);
      int opciones[4] = {0,1,2,3};
      if(validar_respuesta(4,opciones,client_message)){
        printf("el jugador 0 selecciono la accion: %s\n", client_message);
        //implementar logica de la accion 
        //turno siguiente jugador
        if (turno_jugador() == jugadores){
          printf("TURNO MONSTER");
          //ejecutar turno monstruo
        }
        impresion_estadisticas();
        seleccion_de_poder(sockets_array[turno_jugador()]);
        turno += 1;
      }
      else{
        char * mensaje = "Input no valido\n";
        server_send_message(s->cfd, 5, mensaje);
        seleccion_de_poder(s->cfd);
      }  
      break;}
    
    }
  }
  free(s);
}
int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

// Se define la estructura para almacenar info del socket del servidor al momento de su creación
  struct sockaddr_in server_addr;

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(PORT);

  // Se le asigna al socket del servidor un puerto y una IP donde escuchar
  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes
  struct sockaddr_in client_addr[4];
  socklen_t addr_size = sizeof(client_addr[0]);
  
  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
  PlayersInfo * players_info = malloc(sizeof(PlayersInfo));

  
  int socket_lider;
  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.

  while (1)
  {
    if (jugadores < 4 ){
      players_info->sockets_c[jugadores] = accept(server_socket, (struct sockaddr *)&client_addr[jugadores], &addr_size);

      s_info *clie_sock = (s_info *)malloc(sizeof(s_info));
      clie_sock->cfd = players_info->sockets_c[jugadores];
      sockets_array[jugadores] = players_info->sockets_c[jugadores];
      clie_sock->clie_addr = client_addr[jugadores];
      clie_sock->num = jugadores;

      if (jugadores == 0){
        clie_sock->lider = 1;
        socket_lider = clie_sock->cfd;
      }
      else{
        clie_sock->lider = 0;
      }
      clie_sock->socket_lider = socket_lider;

      Jugador jugador;
      jugador.activo = 1;
      lista_jugadores[jugadores] = jugador;
      pthread_t id;
      pthread_create(&id,NULL,(void *)thread_cliente,(void *)clie_sock);
      pthread_detach(id);
      jugadores+=1;
    }
    
    
  }

  return 0;
}
