#include "class.h"
#include <math.h>

/* Habilidades Cazador */
void cazador_estocada(Monstruo monstruo)
{
    monstruo.vida = monstruo.vida - 1000;
    if (monstruo.vida < 0)
        monstruo.vida = 0;
    if (monstruo.sangrado < 3)
        monstruo.sangrado++;
    printf("[Cazador] Estocada\n");
}

void cazador_corte_cruzado(Monstruo monstruo)
{
    monstruo.vida = monstruo.vida - 3000;
    if (monstruo.vida < 0)
        monstruo.vida = 0;
    printf("[Cazador] Corte cruzado\n");
}

void cazador_distraer()
{
    printf("[Cazador] Distraer\n");
}

/* Habilidades Medico */
void medico_descarga_vital(Jugador medico, Monstruo monstruo)
{
    monstruo.vida = monstruo.vida - 2 * (medico.vida_maxima - medico.vida);
    if (monstruo.vida < 0)
        monstruo.vida = 0;
    printf("[Medico] Descarga vital\n");
}

void medico_curar(Jugador jugador)
{

    int vida_actual = 0;
    int vida_max = 0;
    vida_max = jugador.vida_maxima;
    vida_actual = jugador.vida + 2000;
    if (vida_actual > vida_max)
        vida_actual = vida_max;
    jugador.vida = vida_actual;
    printf("[Medico] Curar\n");
}

void medico_destello_regenerador(Jugador jugador, Monstruo monstruo)
{
    int ataque = 0;
    ataque = (rand() % (2000 - 750 + 1)) + 750; // Sacado de: https://www.geeksforgeeks.org/generating-random-number-range-c/
    int mas_vida = 0;
    mas_vida = (int)ceil(ataque / 2);
    jugador.vida = jugador.vida + mas_vida;
    monstruo.vida = monstruo.vida - ataque;
    if (monstruo.vida < 0)
        monstruo.vida = 0;
    printf("[Medico] Destello Regenerador\n");
}

/* Habilidades Hacker */
int hacker_fuerza_bruta(Jugador *hacker)
{
    if (hacker)
    {
        if (strcmp(hacker->clase, "hacker"))
        {
            if (hacker->fuerza_bruta < 2)
            {
                hacker->fuerza_bruta++;
                return 0;
            }
            hacker->fuerza_bruta = 0;
            printf("[Hacker] Fuerza bruta\n");
            return 10000;
        }
    }
    return 0;
}

int hacker_ataque_ddos(Jugador *hacker)
{
    if (hacker)
    {
        if (strcmp(hacker->clase, "hacker"))
        {
            printf("[Hacker] Ataque DDOS\n");
            return 1500;
        }
    }
    return 0;
}