#include "APIParte3.h"
#include "stdio.h"

u32 checkear_coloreo(Grafo G, u32 *Color)
{
    // O(m)
    for (u32 index = 0; index < NumeroDeVertices(G); ++index)
    {
        if (Color[index] == NULL_COLOR)
        { // si hay un vertice que no tiene color esta mal 
            return 1;
        }
        for (u32 vecino = 0; vecino < Grado(index, G); ++vecino)
        {
            if (Color[index] == Color[IndiceVecino(vecino, index, G)]) 
            {
                return 2;
            }
        }
    }
    return 0;
}

int main()
{
    Grafo g = ConstruirGrafo();
    // DestruirGrafo(g);
    printf("se termino de cargar\n");

    u32 n = NumeroDeVertices(g);
    u32 *color = calloc(n, sizeof(u32));
    u32 *orden = calloc(n, sizeof(u32));

    // orden natural
    for (u32 i = 0; i < n; i++)
    {
        orden[i] = i;
    }

    printf("Fin inicializaciones \n");
    u32 ji = GreedyDinamico(g, orden, color, n-10);

    printf("coloreo :\n");
    for (u32 i = 0; i < n; i++)
    {
        printf("%u ", color[i]);
    }
    printf("\n\n");

    if (ji == NULL_COLOR)
    {
        printf("ERROR HACIENDO GREEDY\n");
        return 0;
    }

    printf("Fin greedy \n");
    u32 check = checkear_coloreo(g, color);
    if (check == 0)
    {
        printf("Coloreo Propio: X(G) ~ %u\n", ji);
    }
    else
    {
        if (check == 1)
            printf("Coloreo No propio, hay vertices sin colorear , %u\n", ji);
        else
            printf("Coloreo no propio, hay vertices conectados con el mismo color \n");
    }

    char c = SecondOrder(g, orden, color);
    printf("Reorder hecho una vez: %s\n", c == '0' ? "SUCCESS" : "FAILURE");


    printf("orden :\n");
    for (u32 i = 0; i < n; i++)
    {
        printf("%u ", orden[i]);
    }
    printf("\n\n");

    ji = GreedyDinamico(g, orden, color, n + 1);
    
    printf("coloreo :\n");
    for (u32 i = 0; i < n; i++)
    {
        printf("%u ", color[i]);
    }
    printf("\n\n");

    printf("Fin greedy \n");
    if (checkear_coloreo(g, color))
    {
        printf("Coloreo Propio: X(G) ~ %u\n", ji);
    }
    else
    {
        if (check == 1)
            printf("Coloreo No propio, hay vertices sin colorear , %u\n", ji);
        else
            printf("Coloreo no propio, hay vertices conectados con el mismo color \n");
    }


    DestruirGrafo(g);
    return 0;
}