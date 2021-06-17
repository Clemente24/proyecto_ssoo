#include "class.h"
#include <math.h>

/* ---- CLASES ----*/

int calculo_ataque(Jugador *jugador, int ataque)
{
    if (jugador->inyeccion_sql > 0)
    {
        jugador->inyeccion_sql--;
        return 2 * ataque;
    }
    return ataque;
}

/* Habilidades Cazador */
void cazador_estocada(Jugador *jugador, Monstruo *monstruo)
{
    int ataque = calculo_ataque(jugador, 1000);
    monstruo->vida = monstruo->vida - ataque;
    if (monstruo->vida < 0)
        monstruo->vida = 0;
    if (monstruo->sangrado < 3)
        monstruo->sangrado++;
    printf("[Cazador] Estocada\n");
}

void cazador_corte_cruzado(Jugador *jugador, Monstruo *monstruo)
{
    int ataque = calculo_ataque(jugador, 3000);
    monstruo->vida = monstruo->vida - ataque;
    if (monstruo->vida < 0)
        monstruo->vida = 0;
    printf("[Cazador] Corte cruzado\n");
}

/* Habilidades Medico */
void medico_curar(Jugador *medico, Jugador *jugador)
{
    int vida_actual = 0;
    int curar = calculo_ataque(medico, 2000);
    vida_actual = jugador->vida + curar;
    if (vida_actual > jugador->vida_maxima)
        vida_actual = jugador->vida_maxima;
    jugador->vida = vida_actual;
    printf("[Medico] Curar\n");
}

void medico_destello_regenerador(Jugador *medico, Jugador *jugador, Monstruo *monstruo)
{
    int ataque = 0;
    ataque = (rand() % (2000 - 750 + 1)) + 750; // Sacado de: https://www.geeksforgeeks.org/generating-random-number-range-c/
    int ataque_bono = calculo_ataque(medico, ataque);
    monstruo->vida = monstruo->vida - ataque_bono;
    int mas_vida = 0;
    mas_vida = (int)ceil(ataque_bono / 2);
    jugador->vida = jugador->vida + mas_vida;
    if (monstruo->vida < 0)
        monstruo->vida = 0;
    if (jugador->vida > jugador->vida_maxima)
        jugador->vida = jugador->vida_maxima;
    printf("[Medico] Destello Regenerador\n");
}

void medico_descarga_vital(Jugador *jugador, Monstruo *monstruo)
{
    int descarga = 0;
    descarga = 2 * (jugador->vida_maxima - jugador->vida);
    int ataque = calculo_ataque(jugador, descarga);
    monstruo->vida = monstruo->vida - ataque;
    if (monstruo->vida < 0)
        monstruo->vida = 0;
    printf("[Medico] Descarga vital\n");
}

/* Habilidades Hacker */
void hacker_inyeccion_sql(Jugador *jugador)
{
    jugador->inyeccion_sql = 2;
    printf("[Hacker] Inyeccion SQL\n");
}

void hacker_ataque_ddos(Jugador *jugador, Monstruo *monstruo)
{
    int ataque = calculo_ataque(jugador, 1500);
    printf("Daño: %i\n", ataque);
    monstruo->vida = monstruo->vida - ataque;
    if (monstruo->vida < 0)
        monstruo->vida = 0;
    printf("[Hacker] Ataque DDOS\n");
}

void hacker_fuerza_bruta(Jugador *jugador, Monstruo *monstruo)
{
    if (jugador->fuerza_bruta < 2)
    {
        jugador->fuerza_bruta++;
    }
    else
    {
        jugador->fuerza_bruta = 0;
        int ataque = calculo_ataque(jugador, 10000);
        monstruo->vida = monstruo->vida - ataque;
        if (monstruo->vida < 0)
            monstruo->vida = 0;
    }
    printf("[Hacker] Fuerza bruta\n");
}

//
//
/* ---- MONSTRUO ----*/
int m_calculo_ataque(Monstruo *monstruo, int ataque)
{
    if (monstruo->inyeccion_sql > 0)
    {
        monstruo->inyeccion_sql--;
        return 2 * ataque;
    }
    return ataque;
}

/* Habilidades Monstruo (copia) */
void monstruo_estocada(Monstruo *monstruo, Jugador *jugador)
{
    int ataque = m_calculo_ataque(monstruo, 1000);
    jugador->vida = jugador->vida - ataque;
    if (jugador->vida < 0)
        jugador->vida = 0;
    if (jugador->sangrado < 3)
        jugador->sangrado++;
    printf("[Monstruo] Estocada\n");
}

void monstruo_corte_cruzado(Monstruo *monstruo, Jugador *jugador)
{
    int ataque = m_calculo_ataque(monstruo, 3000);
    jugador->vida = jugador->vida - ataque;
    if (jugador->vida < 0)
        jugador->vida = 0;
    printf("[Monstruo] Corte cruzado\n");
}

void monstruo_distraer()
{
    printf("[Monstruo] Distraer\n");
}

void monstruo_curar(Monstruo *monstruo)
{
    int vida_actual = 0;
    int curar = m_calculo_ataque(monstruo, 2000);
    vida_actual = monstruo->vida + curar;
    if (vida_actual > monstruo->vida_maxima)
        vida_actual = monstruo->vida_maxima;
    monstruo->vida = vida_actual;
    printf("[Monstruo] Curar\n");
}

void monstruo_destello_regenerador(Monstruo *monstruo, Jugador *jugador)
{
    int ataque = 0;
    ataque = (rand() % (2000 - 750 + 1)) + 750; // Sacado de: https://www.geeksforgeeks.org/generating-random-number-range-c/
    int ataque_bono = m_calculo_ataque(monstruo, ataque);
    jugador->vida = jugador->vida - ataque_bono;
    int mas_vida = 0;
    mas_vida = (int)ceil(ataque_bono / 2);
    monstruo->vida = monstruo->vida + mas_vida;
    if (jugador->vida < 0)
        jugador->vida = 0;
    if (monstruo->vida > monstruo->vida_maxima)
        monstruo->vida = monstruo->vida_maxima;
    printf("[Monstruo] Destello Regenerador\n");
}

void monstruo_descarga_vital(Monstruo *monstruo, Jugador *jugador)
{
    int descarga = 0;
    descarga = 2 * (monstruo->vida_maxima - monstruo->vida);
    int ataque = m_calculo_ataque(monstruo, descarga);
    jugador->vida = jugador->vida - ataque;
    if (jugador->vida < 0)
        jugador->vida = 0;
    printf("[Monstruo] Descarga vital\n");
}

void monstruo_inyeccion_sql(Monstruo *monstruo)
{
    monstruo->inyeccion_sql = 2;
    printf("[Monstruo] Inyeccion SQL\n");
}

void monstruo_ataque_ddos(Monstruo *monstruo, Jugador *jugador)
{
    int ataque = m_calculo_ataque(monstruo, 1500);
    jugador->vida = jugador->vida - ataque;
    if (jugador->vida < 0)
        jugador->vida = 0;
    printf("[Monstruo] Ataque DDOS\n");
}

void monstruo_fuerza_bruta(Monstruo *monstruo, Jugador *jugador)
{
    if (monstruo->fuerza_bruta < 3)
    {
        monstruo->fuerza_bruta++;
    }
    else
    {
        monstruo->fuerza_bruta = 0;
        int ataque = m_calculo_ataque(monstruo, 10000);
        jugador->vida = jugador->vida - ataque;
        if (jugador->vida < 0)
            jugador->vida = 0;
    }
    printf("[Monstruo] Fuerza bruta\n");
}