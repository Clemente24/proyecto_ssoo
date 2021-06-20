#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "comunication.h"
#include "conection.h"
#include "class.h"
#include <time.h>
#include <strings.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int inicio_juego = 0;
int turno = 0;
// numero jugadores
int jugadores = 0;
// cantidad de jugadores activos
int jugadores_activos = 0;

//Cantidad de jugadores desconectados
int desconectados = 0;
// indice en la lista de jugadores del jugador activo(su turno)
int jugador_activo = 0;
int sockets_array[4];
int poderes_array[9];


typedef struct sock_info
{
  int cfd;
  struct sockaddr_in clie_addr;
  int num;
  int lider;
  int socket_lider;
} s_info;

Jugador lista_jugadores[4];
Monstruo monstruo;

int validar_respuesta(int n_opciones, int opciones[], char *client_message)
{
  int respuesta = *client_message - '0';

  for (int i = 0; i < n_opciones; i++)
  {
    if (opciones[i] == respuesta)
    {
      return 1;
    }
  }
  return 0;
}
void seleccionar_clase(char *client_message, int indice)
{
  int clase = *client_message - '0';
  if (clase == 1)
  {
    lista_jugadores[indice].clase = "Cazador";
    lista_jugadores[indice].vida = 5000;
    lista_jugadores[indice].vida_maxima = 5000;
    poderes_array[0]=1;
    poderes_array[1]=1;
    poderes_array[2]=1;
  }

  else if (clase == 2)
  {
    lista_jugadores[indice].clase = "Medico";
    lista_jugadores[indice].vida = 3000;
    lista_jugadores[indice].vida_maxima = 3000;
    poderes_array[3]=1;
    poderes_array[4]=1;
    poderes_array[5]=1;
  }

  else
  {
    lista_jugadores[indice].clase = "Hacker";
    lista_jugadores[indice].vida = 2500;
    lista_jugadores[indice].vida_maxima = 2500;
    poderes_array[6]=1;
    poderes_array[7]=1;
    poderes_array[8]=1;
  }
  lista_jugadores[indice].distraer = 0;
  lista_jugadores[indice].fuerza_bruta = 0;
  lista_jugadores[indice].inyeccion_sql = 0;
  lista_jugadores[indice].reprobado = 0;
  lista_jugadores[indice].sangrado = 0;
  lista_jugadores[indice].infected = 0;
}

void seleccionar_monstruo(char *client_message)
{
  int tipo = *client_message - '0';
  //si la seleccion fue aleatoria
  if (tipo == 4)
  {
    //elije numero entre 1 y 3
    tipo = rand() % 3 + 1;
  }
  if (tipo == 1)
  {
    monstruo.tipo = "Great JagRuz";
    monstruo.vida = 10000;
    monstruo.vida_maxima = 10000;
  }
  if (tipo == 2)
  {
    monstruo.tipo = "Ruzalos";
    monstruo.vida = 20000;
    monstruo.vida_maxima = 20000;
  }
  if (tipo == 3)
  {
    monstruo.tipo = "Ruiz";
    monstruo.vida = 25000;
    monstruo.vida_maxima = 25000;
  }
  monstruo.inyeccion_sql = 0;
  monstruo.fuerza_bruta = 0;
  monstruo.sangrado = 0;
  monstruo.salto = 0;
}
int es_turno_monster()
{
  printf("Jugador activo:%i jugadores:%i\n", jugador_activo, jugadores);
  if (jugador_activo == jugadores)
  {
    return 1;
  }
  else
  {
    for (int i = jugador_activo + 1; i < jugadores; i++)
    {
      if (lista_jugadores[i].activo == 1)
      {
        return 0;
      }
    }
    return 1;
  }
}


void enviar_mensaje_a_todos(char *msj)
{
  for (int i = 0; i < jugadores; i++)
  {
    if (lista_jugadores[i].conectado){
      server_send_message(sockets_array[i], 5, msj);
    }
    
  }
}

void atack(int player, int vidas)
{
  if (lista_jugadores[player].distraer)
  { //Caso tienen poder distraer 1=lo tiene, 2=ultimo en tenerlo, 0 no lo tiene
    for (int i = 0; i < jugadores; i++)
    {
      if (lista_jugadores[i].distraer == 2 && lista_jugadores[i].activo == 1)
      {
        lista_jugadores[i].distraer = 0;
        atack(i, vidas);
      }
    }
    lista_jugadores[player].distraer = 2;
  }
  else
  {
    if (lista_jugadores[player].reprobado){
      lista_jugadores[player].vida -= vidas*1.5;
      } else {
        lista_jugadores[player].vida -= vidas;
      }
  }
}
void atack_all(int vidas)
{
  for (int i = 0; i < jugadores; i++)
  {
    if (lista_jugadores[i].activo)
    {
      if (lista_jugadores[i].distraer == 1)
      {
        for (int j = 0; j < jugadores; j++)
        {
          if (lista_jugadores[j].distraer == 2 && lista_jugadores[j].activo == 1)
          {
            lista_jugadores[j].distraer = 0;
          }
        }
        lista_jugadores[i].distraer = 2;
      }
      if (lista_jugadores[i].reprobado){
      lista_jugadores[i].vida -= vidas*1.5;
      } else {
        lista_jugadores[i].vida -= vidas;
      }
    }
  }
  printf("atacando");
}

void jagruz_atack()
{
  int num;
  num = rand() % 10;
  printf("%d \n", num);
  if (num < 5)
  {
    int jug;
    jug = rand() % jugadores_activos;
    for (int i = 0; i < jugadores; i++)
    {
      if (lista_jugadores[i].activo)
      {
        if (jug == i)
        {
          atack(jug, 1000);
          char mensaje[100];
          sprintf(mensaje, "Great JagRuz le da un RazJuñazo a %s!\n", lista_jugadores[jug].nombre);
          enviar_mensaje_a_todos(mensaje);
        }
      }
      else
      {
        jug += 1;
      }
    }
  }
  else
  {
    atack_all(500);
    char mensaje[100];
    sprintf(mensaje, "Great JagRuz les da un RazColetazo a todos!\n");
    enviar_mensaje_a_todos(mensaje);
  }
}

void ruzalos_atack()
{
  int num;
  num = rand() % 10;
  printf("%d \n", num);
  if (num < 4 && monstruo.salto == 0)
  {
    int jug;
    jug = rand() % jugadores_activos;
    for (int i = 0; i < jugadores; i++)
    {
      if (lista_jugadores[i].activo)
      {
        if (jug == i)
        {
          atack(jug, 1500);
          char mensaje[100];
          sprintf(mensaje, "Ruzalos salta sobre %s!\n", lista_jugadores[jug].nombre);
          enviar_mensaje_a_todos(mensaje);
        }
      }
      else
      {
        jug += 1;
      }
    }
    monstruo.salto = 1;
  }
  else
  {
    monstruo.salto = 0;
    int jug;
    jug = rand() % jugadores_activos;
    for (int i = 0; i < jugadores; i++)
    {
      if (lista_jugadores[i].activo)
      {
        if (jug == i)
        {
          if (lista_jugadores[jug].infected > 0)
          {
            atack(jug, 500);
            //lista_jugadores[jug].infected -= 1;
            char mensaje[100];
            sprintf(mensaje, "%s recibe daño por doble espina!\n", lista_jugadores[jug].nombre);
            enviar_mensaje_a_todos(mensaje);
          }
          else
          {
            atack(jug, 400);
            lista_jugadores[jug].infected = 2;
            char mensaje[100];
            sprintf(mensaje, "Ruzalos ha envenenado a %s!\n", lista_jugadores[jug].nombre);
            enviar_mensaje_a_todos(mensaje);
          }
        }
      }
      else
      {
        jug += 1;
      }
    }
  }
}

void ruiz_atack(){
    int num;
     num = rand() % 10;
     printf("%d \n",num);
     int jug;
        jug = rand() % jugadores_activos;
        for (int i = 0; i < jugadores; i++)
            {
              if (lista_jugadores[i].activo)
              {
                if (jug == i)
                {
                  break;
                }
              }
              else
              {
                jug += 1;
              }
            }
     if (num<4){
         //Copiar habilidad de un jugador
         int habilidad;
         int ok =1;
        habilidad = rand() % 9;
        while(ok) {
          if (poderes_array[habilidad]==1){
            ok=0;
          } else {
            habilidad = rand() % 9;
          }
        }
        
        if (habilidad==0){
          monstruo_estocada(&monstruo, &lista_jugadores[jug]);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza estocada!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==1){
          monstruo_corte_cruzado(&monstruo,&lista_jugadores[jug]);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza corte cruzado!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==2){
          monstruo_distraer();
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza distraer!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==3){
          monstruo_curar(&monstruo);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza curar!\n");
          enviar_mensaje_a_todos(mensaje);          
        }
        if (habilidad==4){
          monstruo_destello_regenerador(&monstruo,&lista_jugadores[jug]);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza destello regenerador!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==5){
          monstruo_descarga_vital(&monstruo,&lista_jugadores[jug]);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza descarga vital!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==6){
          monstruo_inyeccion_sql(&monstruo);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza inyeccion sql!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==7){
          monstruo_ataque_ddos(&monstruo, &lista_jugadores[jug]);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza ataque DDoS!\n");
          enviar_mensaje_a_todos(mensaje);
        }
        if (habilidad==8){
          monstruo_fuerza_bruta(&monstruo, &lista_jugadores[jug]);
          char mensaje[100];
          sprintf(mensaje, "Ruiz utiliza fuerza bruta!\n");
          enviar_mensaje_a_todos(mensaje);
        }
     } else{
         if (num<6){
           lista_jugadores[jug].reprobado=1;
            char mensaje[100];
            sprintf(mensaje, "Ruiz a reprobado a %s!\n", lista_jugadores[jug].nombre);
            enviar_mensaje_a_todos(mensaje);
     } else{        //ultimo caso
          atack_all(100*turno);
          char mensaje[100];
          sprintf(mensaje, "Ruiz a escrito en la consola sudo rm -rf!\n");
          enviar_mensaje_a_todos(mensaje);
     }
     }
}

int proximo_jugador()
{
  // caso de hay solo un jugador
//   printf("Jugadores activos: %i\n", jugadores_activos);
  if (jugadores_activos == 1)
  {
    for (int i = 0; i < jugadores; i++)
    {
      if (lista_jugadores[i].activo == 1)
      {
        return i;
      }
    }
  }
  //caso de hay mas de un jugador
  //comenzamos desde el siguiente jugador despues del actual a revisar
  printf("Jugador_activo + 1 %i\n", jugador_activo + 1);

  int j = jugador_activo + 1;
  while (1)
  {
    // // revisamos solo los activos
    printf("j: %i\n", j);
    printf("j mod jugadores %i\n", j % jugadores);
    if (lista_jugadores[j % jugadores].activo == 1)
    {

      return j % jugadores;
    }
    j++;
  }
}

void seleccion_de_poder(int socket)
{
  Jugador jugador_actual = lista_jugadores[jugador_activo];
  if (jugador_actual.clase == "Cazador")
  {
    char *mensaje = "Elije una accion:\n[0]Rendirse\n[1]Estocada\n[2]Corte Cruzado\n[3]Distraer\n";
    server_send_message(socket, 4, mensaje);
  }
  else if (jugador_actual.clase == "Medico")
  {
    char *mensaje = "Elije una accion:\n[0]Rendirse\n[1]Curar\n[2]Destello Regenerador\n[3]Descarga Vital\n";
    server_send_message(socket, 4, mensaje);
  }
  else
  {
    char *mensaje = "Elije una accion:\n[0]Rendirse\n[1]Inyeccion SQL\n[2]Ataque DDOS\n[3]Fuerza Bruta\n";
    server_send_message(socket, 4, mensaje);
  }
}


void ejecutar_poder(Jugador jugador, char *client_message)
{
  int seleccion = *client_message - '0';
  Monstruo *puntero_monstruo = &(monstruo);
  Jugador *puntero_jugador = &(lista_jugadores[jugador_activo]);
  if (seleccion == 0)
  {

    lista_jugadores[jugador_activo].activo = 0;
    lista_jugadores[jugador_activo].vida = 0;
    jugadores_activos -= 1;
    char str_rendirse[30];
    sprintf(str_rendirse, "El jugador %s se ha RENDIDO!!!\n", jugador.nombre);
    enviar_mensaje_a_todos(str_rendirse);
  }

  if (jugador.clase == "Cazador")
  {
    if (seleccion == 1)
      cazador_estocada(puntero_jugador, puntero_monstruo); // Monstruo debe controlar su sangrado
    else if (seleccion == 2)
      cazador_corte_cruzado(puntero_jugador, puntero_monstruo); // Ok
    else if (seleccion == 3)
    {
      jugador.distraer = 1;
      char mensaje[30] = "[Cazador] Distraer\n";
      enviar_mensaje_a_todos(mensaje);
      printf("[Cazador] Distraer\n");
    }
  }
  else if (jugador.clase == "Medico")
  {
    if (seleccion == 1)
      medico_curar(puntero_jugador, &(jugador)); // "Jugador 2" debe ser elegido (Corregir)
    else if (seleccion == 2)
    {
      int loop = 1;
      Jugador *puntero_otro_jugador;
      while (loop)
      {
        int indice = rand() % jugadores;
        puntero_otro_jugador = &(lista_jugadores[indice]);
        if (puntero_otro_jugador->activo == 1)
          loop = 0;
      }
      medico_destello_regenerador(puntero_jugador, puntero_otro_jugador, puntero_monstruo); // OK
    }
    else if (seleccion == 3)
      medico_descarga_vital(puntero_jugador, puntero_monstruo); // OK
  }
  else
  {
    if (seleccion == 1)
      hacker_inyeccion_sql(puntero_jugador); // "Jugador" debe ser elegido (Corregir)
    else if (seleccion == 2)
      hacker_ataque_ddos(puntero_jugador, puntero_monstruo); // OK
    else if (seleccion == 3)
      hacker_fuerza_bruta(puntero_jugador, puntero_monstruo); // OK
  }
}

void turno_monstruo()
{
  char *marco_top = "------------------------TURNO MONSTRUO----------------------------\n";
  enviar_mensaje_a_todos(marco_top);
  if (strcmp(monstruo.tipo, "Great JagRuz") == 0)
  {
    jagruz_atack();
  }
  else if (strcmp(monstruo.tipo, "Ruzalos") == 0)
  {
    ruzalos_atack();
  }
  else
  {
    ruiz_atack();
  }
}



void impresion_estadisticas(){
  
  for (int i=0; i<jugadores;i++){
      //Mandamos solo a jugadores conectados
      if (lista_jugadores[i].conectado ){
            char * marco_top = "------------------------ESTADO JUEGO----------------------------\n";
            char * marco_bot = "----------------------------------------------------------------\n";
            server_send_message(sockets_array[i], 5, marco_top);
            char str_turno[100];
            sprintf(str_turno, "Turno numero: %i          Turno de: %s\n", turno, lista_jugadores[jugador_activo].nombre);
            server_send_message(sockets_array[i], 5, str_turno);
            
            char str_final[100];
            sprintf(str_final, "%s             vida: %i/%i\n",monstruo.tipo,monstruo.vida,monstruo.vida_maxima);
            server_send_message(sockets_array[i], 5, str_final);

            server_send_message(sockets_array[i], 5, marco_bot);
      


        for(int j=0; j<jugadores;j++){
            if (lista_jugadores[j].conectado ){
                char str_final[100];
                sprintf(str_final, "%s[%s]             vida: %i/%i\n",lista_jugadores[j].nombre,lista_jugadores[j].clase,
                lista_jugadores[j].vida,lista_jugadores[j].vida_maxima);
                server_send_message(sockets_array[i], 5, str_final);
                server_send_message(sockets_array[i], 5, marco_bot);
            }
        }
        
    }
    
  }
}

void finalizar_juego(){
    jugadores_activos = 0;
    //LE enviamos un mensaje a todos los jugadores para preguntar si desean seguir jugando
    for (int i=0; i< jugadores; i++){
        if(lista_jugadores[i].conectado){
            char * marco_top = "------------------------JUEGO FINALIZADO----------------------------\n*\n*\n*\n";
            server_send_message(sockets_array[i], 5, marco_top);
            // impresion_estadisticas();
            //Ahora Vemos quien quiere seguir jugando
            char * mensaje = "Desea desafiar al siguiente jefe?: \n[1]Si! (Continuar)\n[2]No. (Desconectarse)\n\n";
            server_send_message(sockets_array[i], 6, mensaje);
            //Reseteamos los turnos
            turno = 0;
            //Reseteamos a los jugadores activos
            jugadores_activos = 0;
        }
    }

}

void *thread_cliente(void *arg){
  s_info *s = (s_info *)arg;
  if (s->lider)
  {
    char *mensaje = "Bienvenido Jugador 1 eres el lider del grupo!!\nIngresa tu nombre:";
    server_send_message(s->cfd, 0, mensaje);
  }
  else
  {
    if (!inicio_juego)
    {
      char *mensaje = "Bienvenido Jugador\nIngresa tu nombre:";
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

      char *client_message = server_receive_payload(s->cfd);

      lista_jugadores[s->num].nombre = client_message;
      char *mensaje = "Seleccione la clase: \n[1]Cazador\n[2]Medico\n[3]Hacker\n";
      server_send_message(s->cfd, 1, mensaje);
      break;
    }

    case 1: //recibe clase jugador, si es lider envia soliciutd de inicio juguego, caso contrario envia mensaje de espera
    {
      char *client_message = server_receive_payload(s->cfd);
      int opciones[3] = {1, 2, 3};

      if (validar_respuesta(3, opciones, client_message))
      {
        seleccionar_clase(client_message, s->num);
        // sumamos un jugador a los activos
        jugadores_activos += 1;
        if (s->lider || lista_jugadores[s->num].lider)
        {
          char *mensaje = "Presione enter para iniciar el juego\n";
          server_send_message(s->cfd, 2, mensaje);
          lista_jugadores[s->num].lider = 1;
        }
        else
        {

          lista_jugadores[s->num].lider = 0;
          char *mensaje = "Personaje listo, a esperar que el lider inicie el juego\n";
          server_send_message(s->cfd, 5, mensaje);
          char str_final[100];
          sprintf(str_final, "El jugador: %s de la clase:%s se ha conectado\n", lista_jugadores[s->num].nombre, lista_jugadores[s->num].clase);
          server_send_message(s->socket_lider, 5, str_final);
        }
      }
      else
      {
        char *mensaje = "Input no valido\nSeleccione la clase: \n[1]Cazador\n[2]Medico\n[3]Hacker\n";
        server_send_message(s->cfd, 1, mensaje);
      }
      break;
    }

    case 2: //recibe inicio juego envia tipo de monstruo
    {
      char *client_message = server_receive_payload(s->cfd);
      // printf("Jugadores: %i, jugadores activos: %i\n", jugadores, jugadores_activos);
      // si todos los jugadores han seleccionado nombre y clase
    //   printf("Jugadores:%i DEsconectados:%i \n", jugadores, desconectados);
      if ((jugadores - desconectados) == jugadores_activos)
      {
        inicio_juego = 1;
        char *mensaje = "Seleccione el tipo de monstruo:\n[1]Great JagRuz\n[2]Ruzalos\n[3]Ruiz, el Gemelo Malvado del Profesor Ruz\n[4]Aleatorio\n";
        server_send_message(s->cfd, 3, mensaje);
      }
      else
      {
        char *mensaje = "Hay jugadores en conexion!!, espere un tiempo\nPresione enter para iniciar el juego\n";
        server_send_message(s->cfd, 2, mensaje);
      }

      break;
    }

    case 3:
    { //recibe tipo monstruo envia seleccion de poder

      char *client_message = server_receive_payload(s->cfd);
      int opciones[4] = {1, 2, 3, 4};
      if (validar_respuesta(4, opciones, client_message))
      {
        seleccionar_monstruo(client_message);
        jugador_activo = 0;
        impresion_estadisticas();
        seleccion_de_poder(s->cfd);
        turno += 1;
      }
      else
      {
        char *mensaje = "Input no valido\nSeleccione el tipo de monstruo:\n[1]Great JagRuz\n[2]Ruzalos\n[3]Ruiz, el Gemelo Malvado del Profesor Ruz\n[4]Aleatorio\n";
        server_send_message(s->cfd, 3, mensaje);
      }
      break;
    }
    case 4:
    { //recibe accion envia un turno nuevo a siguiente jugador
      printf("INICIO CASO 4\n");
      char *client_message = server_receive_payload(s->cfd);
      int opciones[4] = {0, 1, 2, 3};
      if (validar_respuesta(4, opciones, client_message))
      {
        //matamos al monstruo para testeo
        // monstruo.vida = 0;

        // printf("Nombre: %s\n Numero: %i", lista_jugadores[s->num].nombre, lista_jugadores[s->num].numero);

        //Si todos mueren, o se rinden, se termina el juego.
        if(jugadores_activos == 0){
            //El juego se termina
            char  mensaje[100];
            sprintf(mensaje,"Han sido vencidos por %s!!!!!\n*\n*\n*\n", monstruo.tipo);
            printf("Jugadores han perdido\n");
            // printf("Jugadores activos: %i\n", jugadores_activos);
            enviar_mensaje_a_todos(mensaje);
            //Codigo finalizar juego
            finalizar_juego();
        }
        //Si el monstruo muere el juego se termina
        else if (monstruo.vida == 0){
            char  mensaje[100];
            sprintf(mensaje,"Han logrado vencer a %s!\n*\n*\n*\n", monstruo.tipo);
            printf("Han logrado vencer a %s!\n", monstruo.tipo );
            enviar_mensaje_a_todos(mensaje);
            //Codigo finalizar juego
            //Reiniciamos a la cantidad de jugadores, los activos son 0
            finalizar_juego();

        }else{
            //Funcion que ejecuta poder
            ejecutar_poder(lista_jugadores[jugador_activo],client_message);


            if (es_turno_monster() && jugadores_activos > 0){
                printf("TURNO MONSTER\n");
                char mensaje [50];
                sprintf(mensaje, "Turno de %s\n\n", monstruo.tipo);
                enviar_mensaje_a_todos(mensaje);
                //ejecutar turno monstruo
                turno_monstruo();

                //Chequeo de status
                int derrotados = 0;
                for (int j = 0; j< jugadores ; j++){
                    // printf("Jugador %s Y su vida %i\n", lista_jugadores[j].nombre, lista_jugadores[j].vida);
                    if (lista_jugadores[j].vida <= 0){
                        derrotados += 1;
                        //Si es que su vida es 0 y sigue activo, lo inactivamos
                        if(lista_jugadores[j].activo){
                            lista_jugadores[j].activo = 0;
                            jugadores_activos -=1;
                            char  mensaje[100];
                            sprintf(mensaje,"El jugador %s a caido!\n", lista_jugadores[j].nombre);
                            enviar_mensaje_a_todos(mensaje);
                        }
                    }
                }
                                
                //Buscamos al primer jugador activo:
                // for(int j = 0; j< jugadores; j++){
                //     if (lista_jugadores[j].activo){
                //         jugador_activo;
                //         printf("Jugador activo siguiente: %i\n", jugador_activo);
                //         j = jugadores;
                //     }
                // }
            }
            
            printf("Cambiando de jugador activo \n");
            if(jugadores_activos > 0){
                jugador_activo = proximo_jugador();                           
                impresion_estadisticas();
                seleccion_de_poder(sockets_array[jugador_activo]);
                turno += 1;
            }
            if(jugadores_activos == 0){
                char  mensaje[100];
                sprintf(mensaje,"Han sido vencidos por %s!!!!!\n*\n*\n*\n", monstruo.tipo);
                printf("Jugadores han perdido\n");
                // printf("Jugadores activos: %i\n", jugadores_activos);
                enviar_mensaje_a_todos(mensaje);
                //Codigo finalizar juego
                finalizar_juego();
            }

        }
      }else{
        char * mensaje = "Input no valido\n";
        server_send_message(s->cfd, 5, mensaje);
        seleccion_de_poder(s->cfd);
      } 
      break;}

    case 6:{//Recibe confirmacion si desea seguir jugando
      char * client_message = server_receive_payload(s->cfd);
      int opciones[2] = {1,2};
      if(validar_respuesta(2,opciones,client_message)){
        int seleccion = *client_message - '0';
        //Ver que hacer con la respuesta
        //Se quiere quedar
        if (seleccion == 1){
            //Asumimos que puede cambiar su nombre
            char * mensaje = "Ingresa un nuevo nombre:";


            //Activamos al jugador
            lista_jugadores[s->num].activo = 1;

            if(lista_jugadores[s->num].lider){
                // printf("%s Es lider\n", lista_jugadores[s->num].nombre);
                s->lider = 1;
            }
            server_send_message(s->cfd, 0, mensaje);
        }else{
            char * mensaje = "Gracias por jugar! Ahora seras desconectado\n\n";
            server_send_message(s->cfd, 5, mensaje);
            //inactivamos al jugador
            lista_jugadores[s->num].activo = 0;
            lista_jugadores[s->num].conectado = 0;

            //Caso si es lider y no esta solo
            if(lista_jugadores[s->num].lider && jugadores > 1){
                s->lider = 0;
                //Buscamos al siguiente lider
                int siguiente = rand() % (jugadores - 1) + 1;//Numero entre 1 y jugadores
                //Si no lo encontramos randommente, buscamos a uno activo:
                if(!lista_jugadores[siguiente].conectado){
                    for (int k = 0; k < jugadores; k++){
                        if (lista_jugadores[k].conectado){
                            siguiente = k;
                        }
                    }
                }
                //Si siguiente es 0, asignamos como 1.
                if(siguiente == 0){
                    siguiente = 1;
                }
                char message[255] = "";
                sprintf(message, "Ahora %s es el lider!\n", lista_jugadores[siguiente].nombre);
                enviar_mensaje_a_todos(message);
                lista_jugadores[siguiente].lider = 1;
                lista_jugadores[s->num].lider = 0;
            }

            char message[255] = "";
            sprintf(message, "El jugador %s Se ha desconectado\n", lista_jugadores[s->num].nombre);
            enviar_mensaje_a_todos(message);

            //Paquete para desconectar:
            desconectados += 1;
            server_send_message(s->cfd, 7, mensaje);
            //Test de free
            free(s);
            pthread_exit(NULL);

        }

      }
      else{
        char * mensaje = "Input no valido\nDesea seguir jugando?: \n[1]Si! (Continuar)\n[2]No. (Desconectarse)\n\n";
        server_send_message(s->cfd, 6, mensaje);
     }
    
     break;}
    }
  }
  free(s);
}



int main(int argc, char *argv[]){
  // Se define una IP y un puerto

  //Valores default
  char * IP = "0.0.0.0";
  int PORT = 8080;
  //Manejo de flags
  int i;
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
      printf ("Para correr, debe llamar de la siguiente manera:\n>./server -i <ip_address> -p <tcp_port>\n\n");
      return 0;
  }
  
  
  //Seteamos randomness con una seed
  srand(time(0));
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
  PlayersInfo *players_info = malloc(sizeof(PlayersInfo));

  int socket_lider;
  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.

  while (1)
  {
    if (jugadores < 4)
    {
      players_info->sockets_c[jugadores] = accept(server_socket, (struct sockaddr *)&client_addr[jugadores], &addr_size);

      s_info *clie_sock = (s_info *)malloc(sizeof(s_info));
      clie_sock->cfd = players_info->sockets_c[jugadores];
      sockets_array[jugadores] = players_info->sockets_c[jugadores];
      clie_sock->clie_addr = client_addr[jugadores];
      clie_sock->num = jugadores;

      if (jugadores == 0)
      {
        clie_sock->lider = 1;
        socket_lider = clie_sock->cfd;
      }
      else
      {
        clie_sock->lider = 0;
      }
      clie_sock->socket_lider = socket_lider;


      Jugador jugador;
      jugador.activo = 1;
      jugador.conectado = 1;
      lista_jugadores[jugadores] = jugador;
      if(!clie_sock ->lider){
          lista_jugadores[jugadores].lider = 0;
      }
      pthread_t id;
      pthread_create(&id, NULL, (void *)thread_cliente, (void *)clie_sock);
      pthread_detach(id);
      jugadores += 1;
    }
  }

  free(players_info);


  return 0;
}
