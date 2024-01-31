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

    u32 p=n;
    u32 check;
    u32 ji;
    char c;

    printf("Fin inicializaciones \n");
    


    for (u32 i = 0; i < 1000; i++)
    {

        ji = GreedyDinamico(g, orden, color,p);
        p++;
        if (i == 0)
            printf("initial Ji: %u\n", ji);
        
        

        if (ji == ERROR_CODE){
            printf("ERROR HACIENDO GREEDY\n");
            return 0;}

        check = checkear_coloreo(g, color);

        if (check != 0){
            if (check == 1)
                printf("Coloreo No propio, hay vertices sin colorear , X(G) ~ %u\n", ji);

            if (check == 2)
                printf("Coloreo no propio, hay vertices conectados con el mismo color \n");

            return 0;}

        if (i%2 == 0)
        {
            c = FirstOrder(g, orden, color);
        }else
        {
            c = SecondOrder(g, orden, color);
        }
        
        if (c != '0')
        {
            printf("Reorder hecho una vez: %s\n", c == '0' ? "SUCCESS" : "FAILURE");    
        }
    
    }
    
    printf("last Ji: %u\n\n", ji);
    
    
    // printf("Fin greedy \n");
    // printf("Reorder hecho una vez: %s\n", c == '0' ? "SUCCESS" : "FAILURE");
    
    



    
    // if (ji == ERROR_CODE)
    // {
    //     printf("ERROR HACIENDO GREEDY\n");
    //     // return 0;
    // }
    
    // printf("Fin greedy \n");
    // check = checkear_coloreo(g, color);
    // if (check == 0)
    // {
    //     printf("Coloreo Propio: X(G) ~ %u\n", ji);
    // }
    // else
    // {
    //     if (check == 1)
    //         printf("Coloreo No propio, hay vertices sin colorear , X(G) ~ %u\n", ji);
    //     else
    //         printf("Coloreo no propio, hay vertices conectados con el mismo color \n");
    // }


    // c = SecondOrder(g, orden, color);
    // printf("Reorder hecho una vez: %s\n", c == '0' ? "SUCCESS" : "FAILURE");


    


    DestruirGrafo(g);
    return 0;
}