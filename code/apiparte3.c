#include "APIG23.h"
#include "APIParte3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define USED_COLOR 1

struct nc_data
{
    u32 coloreado;
    u32 valor;
};

// Complejidad O(1)
static void swap(u32 i, u32 j, u32 *array)
{
    u32 temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

// Complejidad O(n)
static u32 cuenta_colores_usados_por_vecinos(u32 *colores_usados, u32 N)
{
    u32 num_colores_usados = 0;
    for (u32 i = 0; i < N; i++)
    {
        num_colores_usados += colores_usados[i];
    }

    return num_colores_usados;
}

// Complejidad O(2n)
static void popular_colores_usados_por_vecinos(Grafo G, u32 indice, u32 *Color, u32 *colores_usados)
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
}

// Complejidad O(3n)
static u32 coloreo(Grafo G, u32 objetivo, u32 *Color, u32 cota)
{
    u32 color_a_usar = NULL_COLOR;

    // vector binario -> 0 - no esta usado | 1 - algun vecino lo usa
    u32 *colores_usados = calloc(NumeroDeVertices(G), sizeof(u32));

    // Complejidad O(2n)
    popular_colores_usados_por_vecinos(G, objetivo, Color, colores_usados);

    // Complejidad O(n)
    for (u32 i = 0; i < NumeroDeVertices(G); i++)
    {
        if (i < cota)
        {
            if (colores_usados[i] == 0)
            {
                color_a_usar = i;
                break;
            }
        }
        else
        {
            color_a_usar = NULL_COLOR;
            break;
        }
    }
    free(colores_usados);
    colores_usados = NULL;
    return color_a_usar;
}

// Complejidad O(delta*(3n)) -> tiende a n*n
static void nc_actualizar(Grafo G, u32 *Color, struct nc_data *NC, u32 vertice_foco)
{
    u32 N = NumeroDeVertices(G);
    u32 *colores_usados = calloc(N, sizeof(u32));

    NC[vertice_foco].coloreado = 1;

    for (u32 j = 0; j < Grado(vertice_foco, G); j++)
    {
        u32 indice_vecino = IndiceVecino(j, vertice_foco, G);
        if (!NC[indice_vecino].coloreado)
        {
            // Complejidad O(2n)
            popular_colores_usados_por_vecinos(G, indice_vecino, Color, colores_usados);
            // Complejidad O(n)
            NC[indice_vecino].valor = cuenta_colores_usados_por_vecinos(colores_usados, N);
        }
    }

    free(colores_usados);
    colores_usados = NULL;
}

// Complejidad O(n*(3n))
static void nc_inicializacion(Grafo G, u32 *Color, struct nc_data *NC)
{
    u32 N = NumeroDeVertices(G);
    u32 *colores_usados = calloc(N, sizeof(u32));

    for (u32 i = 0; i < N; i++)
    {
        if (Color[i] != NULL_COLOR)
        {
            NC[i].coloreado = 1;
        }
        else
        {
            popular_colores_usados_por_vecinos(G, i, Color, colores_usados);
            NC[i].valor = cuenta_colores_usados_por_vecinos(colores_usados, N);
        }
    }

    free(colores_usados);
    colores_usados = NULL;
}

// Complejidad O(n)
static u32 obtener_mejor_nc(struct nc_data *NC, u32 *Orden, u32 N)
{
    u32 mejor_nc_indice = ERROR_CODE;
    u32 mejor_nc_valor = 0;
    u32 mejor_nc_valor_init = 0;

    for (u32 i = 0; i < N; i++)
    {
        if ((!NC[Orden[i]].coloreado && mejor_nc_valor < NC[Orden[i]].valor) ||
            (mejor_nc_indice == ERROR_CODE && !NC[Orden[i]].coloreado && !mejor_nc_valor_init))
        {
            mejor_nc_valor_init = 1;
            mejor_nc_indice = i;
            mejor_nc_valor = NC[Orden[i]].valor;
        }
    }
    return mejor_nc_indice;
}

// Complejidad greedy comun O(n + (n*3n))                                 -> O(n^2)
// Complejidad greedy comun O(n + n*(3*n*n + delta * 3n + 4n + 1))    -> O(n^3)
u32 GreedyDinamico(Grafo G, u32 *Orden, u32 *Color, u32 p)
{
    u32 N = NumeroDeVertices(G);
    u32 max_color = 0;
    struct nc_data *NC = NULL;

    for (u32 i = 0; i < N; i++)
        Color[i] = NULL_COLOR;
  
    // "si p es igual a 0, entonces se debe considerar como si p fuera 1,
    // porque para calcular la parte dinamica hace falta al menos un vertice coloreado"
    p = p == 0 ? 1 : p;
    if (p < N)
        NC = calloc(N, sizeof(struct nc_data));

    // Greedy comun O(n*3n)                         -> O(n^2)
    // Greedy dinamico O(n*(3nn+1+4n+delta*3n))   -> O(n^3)
    for (u32 i = 0; i < N; i++)
    {
        // Complejidad O(3*n*n)
        if (i == p)
            nc_inicializacion(G, Color, NC);

        u32 mejor_nc;
        // Complejidad O(n+1)
        if (p <= i)
        {
            mejor_nc = obtener_mejor_nc(NC, Orden, N);

            if (mejor_nc == ERROR_CODE)
                return ERROR_CODE;

            swap(i, mejor_nc, Orden);
        }

        // Complejidad O(3n)
        u32 vertice_por_colorear = Orden[i];
        u32 color_usado = coloreo(G, vertice_por_colorear, Color, max_color + 2);

        if (color_usado == NULL_COLOR)
            free(NC);
            NC = NULL;
            return ERROR_CODE;
        if (Color[vertice_por_colorear] != NULL_COLOR)
            free(NC);
            NC = NULL;
            return ERROR_CODE;

        if (max_color < color_usado)
            max_color = color_usado;

        Color[vertice_por_colorear] = color_usado;

        // Complejidad O(delta*3n)
        if (p <= i)
            nc_actualizar(G, Color, NC, vertice_por_colorear);
    }
    free(NC);
    NC = NULL;
    return max_color + 1;
}

// COMIENZA FIRST ORDER Complejidad O(n log n)

struct data_fo
{
    u32 cant_vert;
    u32 *vert;
    u32 m;
    u32 M;
    u32 E;
};

static int compar_FO(const void *elem1, const void *elem2)
{
    struct data_fo e1 = *((struct data_fo *)elem1);
    struct data_fo e2 = *((struct data_fo *)elem2);
    if (e2.E < e1.E)
        return -1;
    else if (e2.E > e1.E)
        return 1;
    else
        return 0;
}

char FirstOrder(Grafo G, u32 *Orden, u32 *Color)
{
    u32 n = NumeroDeVertices(G);
    u32 r = 0;

    for (u32 i = 0; i < n; i++)
    {
        if (Color[i] != NULL_COLOR)
        {
            if (r < Color[i] + 1)
            {
                r = Color[i] + 1;
            }
        }
        else
        {
            return '1';
        }
    }
    // r = Ji(G)
    // Complejidad 0(n)

    struct data_fo *data = calloc(r, sizeof(struct data_fo));
    for (u32 i = 0; i < r; i++)
    {
        data[i].cant_vert = 0;
        data[i].vert = calloc(n, sizeof(u32));
        data[i].m = MAX_U32;
        data[i].M = 0;
        data[i].E = 0;
    }
    // data[] inicializado
    // Complejidad O(r)

    for (u32 i = 0; i < n; i++)
    {
        u32 color_i = Color[i];

        if (color_i == NULL_COLOR)
        {
            return '1';
        }

        u32 grado_i = Grado(i, G);

        data[color_i].vert[data[color_i].cant_vert] = i;
        data[color_i].cant_vert++;
        data[color_i].M = data[color_i].M < grado_i ? grado_i : data[color_i].M;
        data[color_i].m = data[color_i].m > grado_i ? grado_i : data[color_i].m;

        if (color_i % 3 == 2)
        {
            data[color_i].E = 0x7fffffff + data[i].m;
        }
        else if (i % 3 == 1)
        {
            data[color_i].E = 0x1fff + data[i].M;
        }
        else
        {
            data[color_i].E = data[i].M + data[i].m;
        }
    }
    // data[] populado
    // Complejidad O(n)

    qsort(data, (size_t)r, sizeof(struct data_fo), compar_FO);
    // data[] ordenado, descendientemente respecto a data[_].E
    // Complejidad O(r log r)

    u32 orden_index = 0;
    for (u32 color_index = 0; color_index < r; color_index++)
    {
        for (u32 v_index = 0; v_index < data[color_index].cant_vert; v_index++)
        {
            Orden[orden_index] = data[color_index].vert[v_index];
            orden_index++;
        }
    }
    // traduccion data -> Orden
    // Complejidad O(n)

    for (u32 i = 0; i < r; i++)
    {
        free(data[i].vert);
    }
    free(data);
    // Limpieza de memoria
    // Complejidad O(r)

    // Complejidad total = O(n + r + n + r log r + n + r)
    // como r <= n
    // queda O(5n + n log n) = O(n log n)
    return '0';
}

// COMIENZA SECOND ORDER Complejidad O(n log n)
struct data_so
{
    u32 cant_vert;
    u32 *vert;
    u32 sum;
};

static int compar_SO(const void *elem1, const void *elem2)
{
    struct data_so e1 = *((struct data_so *)elem1);
    struct data_so e2 = *((struct data_so *)elem2);
    if (e1.sum > e2.sum)
        return -1;
    else if (e2.sum > e1.sum)
        return 1;
    else
        return 0;
}

char SecondOrder(Grafo G, u32 *Orden, u32 *Color)
{
    u32 n = NumeroDeVertices(G);
    u32 r = 0;
    u32 data_length = 0;

    for (u32 i = 0; i < n; i++)
    {
        if (Color[i] != NULL_COLOR)
        {
            if (r < Color[i] + 1)
            {
                r = Color[i] + 1;
            }
        }
        else
        {
            return '1';
        }
    }
    data_length = r + 1;
    // r = cantidad de colores distintos o X(G)
    // Complejidad O(n)

    struct data_so *data = calloc(data_length, sizeof(struct data_so));
    for (u32 i = 0; i < data_length; i++)
    {
        data[i].cant_vert = 0;
        data[i].vert = calloc(n, sizeof(u32));
        data[i].sum = 0;
    }
    // data[] inicializado
    // data{0, ... , r-1}   -> vertices con grado mayor a 1, clasificados en r colores
    // data[r]              -> vertices con grado menor o igual a 1
    // Complejidad O(r+1)

    for (u32 i = 0; i < n; i++)
    {
        u32 color_i = Color[i];
        if (color_i == NULL_COLOR)
        {
            return '1';
        }

        u32 grado_i = Grado(i, G);

        if (grado_i > 1)
        {
            data[color_i].vert[data[color_i].cant_vert] = i;
            data[color_i].cant_vert++;
            data[color_i].sum += grado_i;
        }
        else
        {
            data[r].vert[data[r].cant_vert] = i;
            data[r].cant_vert++;
        }
    }
    // data[] inicializado
    // Complejidad O(r+1)

    qsort(data, (size_t)data_length, sizeof(struct data_so), compar_SO);
    // data[] ordenado descendientemente respecto a sum
    // Complejidad O(r+1  log r+1)

    u32 orden_index = 0;
    for (u32 color_index = 0; color_index < data_length; color_index++)
    {
        for (u32 vert_index = 0; vert_index < data[color_index].cant_vert; vert_index++)
        {
            Orden[orden_index] = data[color_index].vert[vert_index];
            orden_index++;
        }
    }
    // traduccion data -> Orden
    // Complejidad O(n)

    for (u32 i = 0; i < data_length; i++)
    {
        free(data[i].vert);
    }
    free(data);
    // Limpieza de memoria
    // Complejidad O(r+1)

    // Complejidad total = O(n + (r+1) + n + ((r+1) log (r+1)) + n + (r+1))
    // como r <= n
    // queda O(5n + n log n) = O(n log n)
    return '0';
}
