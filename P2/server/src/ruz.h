#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#pragma once


typedef struct monster {
  int init_life;
  int life;
  char name[28];
  int type;
}Monster;

typedef struct game {
    Monster * monster;            //0, 1 o 2 dependiendo del montruo asignado
    int players_alive;
}Game;

Game *game;
void generarMonstruo(int tipo);
void atack(int player, int vidas);
void jagruz_atack();