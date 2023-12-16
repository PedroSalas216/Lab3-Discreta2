#include "APIG23.h"
#include "APIParte3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define USED_COLOR 1

/**
 * @brief Popula el vector binario que indica si los vecinos de el vertice `indice` usaron cada color
 *
 * @param G
 * @param indice
 * @param Color
 * @param colores_usados u32* de tamaño N*(sizeof(u32))
 * @return u32
 */
static u32 popular_colores_usados_por_vecinos(Grafo G, u32 indice, u32 *Color, u32 *colores_usados)
{
    u32 grado = Grado(indice, G);

    // inicializo colores_usados
    for (u32 i = 0; i < NumeroDeVertices(G); i++)
        colores_usados[i] = 0;

    // populo colores_usados
    for (u32 i = 0; i < grado; i++)
    {
        u32 color_vecino = Color[IndiceVecino(i, indice, G)];

        if (color_vecino != NULL_COLOR)
            colores_usados[color_vecino] = USED_COLOR;
    }

    u32 num_colores_usados = 0;
    for (u32 i = 0; i < NumeroDeVertices(G); i++)
    {
        num_colores_usados += colores_usados[i];
    }

    return num_colores_usados;
}

/**
 * @brief Colorea el vertice objetivo y retorna el color que uso
 *
 * @param G
 * @param objetivo
 * @param Color
 * @return u32
 */
static u32 coloreo(Grafo G, u32 objetivo, u32 *Color)
{
    u32 grado = Grado(objetivo, G);
    u32 used_color = 0;

    // vector binario que tiene la siguiente semantica
    // 0 - no esta usado
    // 1 - algun vecino lo usa
    u32 *colores_usados = calloc(NumeroDeVertices(G), sizeof(u32));

    popular_colores_usados_por_vecinos(G, objetivo, Color, colores_usados);

    // encuentro el primer hueco
    u32 i = 0;
    while (i < NumeroDeVertices(G) && colores_usados != NULL && grado)
    {
        if (colores_usados[i] != USED_COLOR)
        {
            used_color = i;
            free(colores_usados);
            colores_usados = NULL;
        }
        ++i;
    }

    return used_color;
}

/**
 * @brief Asume que se llama cuando se colorea el vertice `last_colored_vert`, y procede a actualizar los np de los vertices vecinos
 *
 * @param G
 * @param last_colored_vert
 * @param Color
 * @param NP
 * @return u32
 */
static void np_update(Grafo G, u32 *Color, u32 *NP, u32 focus_vertex)
{

    u32 N = NumeroDeVertices(G);
    u32 *colores_usados = calloc(N, sizeof(u32));

    NP[focus_vertex] = MAX_U32;
    for (u32 j = 0; j < Grado(focus_vertex, G); j++)
    {
        if (Color[j] != NULL_COLOR)
        {
            NP[j] = popular_colores_usados_por_vecinos(G, j, Color, colores_usados);
        }
    }
}

static void np_initialize(Grafo G, u32 *Color, u32 *NP)
{
    u32 N = NumeroDeVertices(G);
    u32 *colores_usados = calloc(N, sizeof(u32));

    for (u32 i = 0; i < N; i++)
    {
        if (Color[i] == NULL_COLOR)
        {
            NP[i] = MAX_U32;
        }
        else
        {
            NP[i] = popular_colores_usados_por_vecinos(G, i, Color, colores_usados);
        }
    }
}

/**
 * @brief Retorna el indice del vertice cuyo np es el mayor
 *
 * @param NP
 * @param N
 * @return u32
 */
static u32 get_best_np(u32 *NP, u32 N)
{
    u32 best_np = 0;
    u32 max_np = 0;
    for (u32 i = 0; i < N; i++)
    {
        if (NP[i] != MAX_U32 && max_np < NP[i])
        {
            best_np = i;
            max_np = NP[i];
        }
    }
    return best_np;
}

u32 GreedyDinamico(Grafo G, u32 *Orden, u32 *Color, u32 p)
{
    // useful vars
    u32 N = NumeroDeVertices(G);
    u32 *NP = calloc(N, sizeof(u32));

    // inicializo Color, NP
    for (u32 i = 0; i < N; i++)
    {
        Color[i] = NULL_COLOR;
        NP[i] = MAX_U32;
    }

    p = p == 0 ? 1 : p;

    for (u32 i = 0; i < N; i++)
    {
        if (i < p)
        {
            coloreo(G, i, Color);
        }
        else
        {
            if (i == p)
                np_initialize(G, Color, NP);
            u32 vertice_dinamico = get_best_np(NP, N);

            coloreo(G, vertice_dinamico, Color);
            np_update(G, Color, NP, vertice_dinamico);
        }
    }
}
