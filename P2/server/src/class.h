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
    int espina_venenosa;
    int reprobado;
} Jugador;

typedef struct monstruo
{
    char *tipo;
    int vida;
    int vida_maxima;
    int sangrado;
} Monstruo;

/* Habilidades Cazador */
void cazador_estocada(Monstruo monstruo); // FALTA CONTROLAR SANGRADO (MONSTRUO)
void cazador_corte_cruzado(Monstruo monstruo);
void cazador_distraer(); // FALTA TERMINAR!

/* Habilidades Medico */
void medico_curar(Jugador jugador); //FALTA QUE SE ELIJA A QUIEN CURAR
void medico_descarga_vital(Jugador medico, Monstruo monstruo);
void medico_destello_regenerador(Jugador jugador, Monstruo monstruo); // FALTA RANDOM JUGADOR

/* Habilidades Hacker */
//void hacker_fuerza_bruta(Jugador *hacker);
//void hacker_ataque_ddos(Jugador *hacker);
//void hacker_inyeccion_sql(Jugador *hacker, Jugador *jugador);