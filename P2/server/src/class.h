#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct clase
{
    int vida;
    int max_vida;
    char *tipo;
    int fuerza_bruta;
    int espina_venenosa;
    int reprobado;
    int inyeccion_sql;
} Clase;

/* Inicializacion clases */
Clase *init_clase(char *tipo);

/* Habilidades Cazador */
int cazador_corte_cruzado(Clase *cazador);
//int cazador_estocada(Clase *cazador);
// int cazador_distraer(Clase * cazador);

/* Habilidades Medico */
int medico_descarga_vital(Clase *medico);
void medico_curar(Clase *medico, Clase *jugador);
// int medico_destello_regenerador(Clase *medico, Clase *jugador);

/* Habilidades Hacker */
int hacker_fuerza_bruta(Clase *hacker);
int hacker_ataque_ddos(Clase *hacker);
//void hacker_inyeccion_sql(Clase *hacker, Clase *jugador);