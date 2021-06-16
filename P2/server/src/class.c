#include "class.h"

/* Inicializacion clases */
Clase *init_cazador(char *tipo)
{
    Clase *mi_clase = malloc(sizeof(Clase));
    if (strcmp(tipo, "cazador"))
    {
        *mi_clase = (Clase){
            .vida = 5000,
            .max_vida = 5000,
            .tipo = tipo,
            .fuerza_bruta = 0,
            .espina_venenosa = 0,
            .reprobado = 0,
            .inyeccion_sql = 0,
        };
    }
    else if (strcmp(tipo, "medico"))
    {
        *mi_clase = (Clase){
            .vida = 3000,
            .max_vida = 3000,
            .tipo = tipo,
            .fuerza_bruta = 0,
            .espina_venenosa = 0,
            .reprobado = 0,
            .inyeccion_sql = 0,
        };
    }
    else if (strcmp(tipo, "hacker"))
    {
        *mi_clase = (Clase){
            .vida = 2500,
            .max_vida = 2500,
            .tipo = tipo,
            .fuerza_bruta = 0,
            .espina_venenosa = 0,
            .reprobado = 0,
            .inyeccion_sql = 0,
        };
    }
    else
    {
        printf("[Init clase] Error de tipo\n");
        return NULL;
    }

    return mi_clase;
}

/* Habilidades Cazador */
int cazador_corte_cruzado(Clase *cazador)
{
    if (cazador)
    {
        if (strcmp(cazador->tipo, "cazador"))
        {
            printf("[Cazador] Corte cruzado\n");
            return 3000;
        }
    }
    return 0;
}

/* Habilidades Medico */
int medico_descarga_vital(Clase *medico)
{
    if (medico)
    {
        if (strcmp(medico->tipo, "medico"))
        {
            printf("[Medico] Descarga vital\n");
            return 2 * (5000 - medico->vida);
        }
    }
    return 0;
}

void medico_curar(Clase *medico, Clase *jugador)
{
    if (medico)
    {
        if (strcmp(medico->tipo, "medico"))
        {
            int vida_actual = 0;
            int vida_max = 0;
            vida_max = jugador->max_vida;
            vida_actual = jugador->vida + 2000;
            if (vida_actual > vida_max)
                vida_actual = vida_max;
            jugador->vida = vida_actual;
            printf("[Medico] Curar\n");
        }
    }
}

/* Habilidades Hacker */
int hacker_fuerza_bruta(Clase *hacker)
{
    if (hacker)
    {
        if (strcmp(hacker->tipo, "hacker"))
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

int hacker_ataque_ddos(Clase *hacker)
{
    if (hacker)
    {
        if (strcmp(hacker->tipo, "hacker"))
        {
            printf("[Hacker] Ataque DDOS\n");
            return 1500;
        }
    }
    return 0;
}