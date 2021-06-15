#include "ruz.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "conection.h"


void generarMonstruo(int tipo){
    int num;
     if (tipo==0){
         num = rand() % 3;
         printf("%d \n",num);
         //game -> monster = num;
     }
     else {
         num = tipo-1;
         //game -> monster = tipo -1:
     }
     printf("Inicio %d \n",num);
     Monster * monster=malloc(sizeof(Monster));
     printf("Inicio tipo %d \n",num);
     monster->type = num;
     printf("Inicio tipo m %d \n",monster->type);
     if (num==0){
         monster -> init_life = 10000;
         monster -> life = 100000;
         strcpy(monster->name, "Great JagRuz");
     } else {
         printf("llegue a else \n");
         if (num==1){
             printf("llegue a crear el %d \n",num);
             monster -> init_life = 20000;
         monster -> life = 200000;
         strcpy(monster->name, "Ruzalos");
         }else {
         if (num==2){
             monster -> init_life = 25000;
         monster -> life = 250000;
         strcpy(monster->name, "Ruiz, el Gemelo Malvado del Profesor Ruz");
         }
     }
   
     }
       printf("guardo %d \n",num);
     //game->monster=monster;
     Game *game = malloc(sizeof(Game));
     *game = (Game){
      .monster = monster,
  };
    printf("monstruo: %s",game->monster->name);

     
}

void atack(int player, int vidas){
    printf("atacando")
}

void jagruz_atack(){
     int num;
     num = rand() % 10;
     printf("%d \n",num);
     if (num<5){
         jug = rand() % game-> players_alive;
         atack(jug,1000);
     } else{
         for (int i=0; i<game-> players_alive; i++){
             atack(i,1000);
         }

     }
     
}

void ruzalos_atack(){
    int num;
     num = rand() % 10;
     printf("%d \n",num);
     if (num<4){
         jug = rand() % game-> players_alive;
         atack(jug,1500);
     } else{
         jug = rand() % game-> players_alive;
         if (game->players[jug].infected>0){
            atack(jug,500);
            game->players[jug]->infected -= 1;
         } else {
             atack(jug,400);
             game->players[jug]->infected = 3;
         }

     }
}
void ruiz_atack(){
    int num;
     num = rand() % 10;
     printf("%d \n",num);
     if (num<4){
         //Copiar habilidad de un jugador
     } else{
         if (num<6){
         jug = rand() % game-> players_alive;
         if (game->players[jug].infected>0){
            atack(jug,500);
            game->players[jug]->infected -= 1;
         } else {
             atack(jug,400);
             game->players[jug]->infected = 3;
         }

     } else{        //ultimo caso

     }
     }
}