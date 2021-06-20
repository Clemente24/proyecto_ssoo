#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct jugador
{
    char *nombre;
    char *clase;
    int lider;
    int vida;
    int vida_maxima;
    int activo;
    int fuerza_bruta;  // Para tipo Hacker, si es >2 inflinge daño
    int inyeccion_sql; // Bonus dado por Hacker, dura 2 turnos
    int reprobado;
    int sangrado;
    int distraer;
    int infected;
    int conectado;//Para diferenciar si estan conectados luego de retirarse del juego.
} Jugador;

typedef struct monstruo
{
    char *tipo;
    int salto;
    int vida;
    int vida_maxima;
    int sangrado;
    int inyeccion_sql;
    int fuerza_bruta;
} Monstruo;

/*----CLASES----*/
/* Habilidades Cazador */
void cazador_estocada(Jugador *jugador, Monstruo *monstruo); // FALTA CONTROLAR SANGRADO (MONSTRUO)
void cazador_corte_cruzado(Jugador *jugador, Monstruo *monstruo);

/* Habilidades Medico */
void medico_curar(Jugador *medico, Jugador *jugador); //FALTA QUE SE ELIJA A QUIEN CURAR
void medico_destello_regenerador(Jugador *medico, Jugador *jugador, Monstruo *monstruo);
void medico_descarga_vital(Jugador *jugador, Monstruo *monstruo);

/* Habilidades Hacker */
void hacker_inyeccion_sql(Jugador *jugador); // FALTA ELEGIR JUGADOR
void hacker_ataque_ddos(Jugador *jugador, Monstruo *monstruo);
void hacker_fuerza_bruta(Jugador *jugador, Monstruo *monstruo);

/*----MONSTRUO----*/
void monstruo_estocada(Monstruo *monstruo, Jugador *jugador); // FALTA CONTROLAR SANGRADO (JUGADOR)
void monstruo_corte_cruzado(Monstruo *monstruo, Jugador *jugador);
void monstruo_distraer();
void monstruo_curar(Monstruo *monstruo);
void monstruo_destello_regenerador(Monstruo *monstruo, Jugador *jugador);
void monstruo_descarga_vital(Monstruo *monstruo, Jugador *jugador);
void monstruo_inyeccion_sql(Monstruo *monstruo);
void monstruo_ataque_ddos(Monstruo *monstruo, Jugador *jugador);
void monstruo_fuerza_bruta(Monstruo *monstruo, Jugador *jugador);
