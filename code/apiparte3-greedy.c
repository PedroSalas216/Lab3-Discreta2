
#include "APIG23.h"
#include "APIParte3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define USED_COLOR 1

static u32 colorear (Grafo g, u32 indice_real ,u32* Color){
    u32 grado = Grado(indice_real, g);
    
    if (grado == 0) return 0;

    u32* colores_usados = calloc(NumeroDeVertices(g), sizeof(u32));
    u32 min_color = NULL_COLOR;

    for(u32 i = 0; i < grado; i++){
        u32 color_vecino = Color[IndiceVecino(i,indice_real,g)];

        if (color_vecino != NULL_COLOR) 
            colores_usados[color_vecino] = USED_COLOR;
    }
    // Complejidad a lo sumo O(n-1)

    u32 i = 0; 
    while (i < NumeroDeVertices(g) && colores_usados != NULL){
        
        // encuentro el primer hueco 
        if (colores_usados[i] != USED_COLOR){
            min_color = i;
            free(colores_usados);
            colores_usados = NULL;
        } else {

        }
        ++i;
    }
    // Complejidad de a lo sumo O(n)
    
    return min_color;
    // Compl total : O(2n)
}

// itera sobre los vecinos del vertice con indice "indice_real"
static u32 single_nc(Grafo g, u32 indice_real, u32* Color){
    u32* colores_usados = calloc(NumeroDeVertices(g),sizeof(u32));
    u32 nc = 0;


    for(u32 i = 0; i < Grado(indice_real,g); i++){
        u32 color_vecino = Color[IndiceVecino(i,indice_real,g)];

        if (color_vecino != NULL_COLOR){
            // Color no nulo
            if (colores_usados[color_vecino] != USED_COLOR) {
                // Color no nulo, no usado
                nc ++;

            }
            colores_usados[color_vecino] = USED_COLOR;
        }

    }
    free(colores_usados);
    return nc;
    // complejidad O(gr(vertice))
}

static u32 nc(Grafo g, u32* Color, u32* Orden, u32 p){
    // hace una busqueda desde p hasta n-1 de el indice_orden cuyo vertice tiene el mayor nc
    u32 max_nc = 0;
    u32 ret = 0;

    for (u32 indice_orden = p; indice_orden < NumeroDeVertices(g); indice_orden++) {
        if (Color[Orden[indice_orden]] == NULL_COLOR) {
            u32 temp_nc = single_nc(g, Orden[indice_orden], Color); 
            if (max_nc < temp_nc) {
                max_nc = temp_nc; 
                ret = indice_orden;
            }
        }        
    }

    return ret;
    // Comlejidad O(gr(vertice) * n-p)
    // complejidad O(n^2)
}

u32 GreedyDinamico(Grafo G, u32* Orden, u32* Color, u32 p){

    u32 color_count = 0;
    u32 n = NumeroDeVertices(G);
   
    for (u32 i = 0; i < n; i++){
        Color[i] = NULL_COLOR;
    }
    // Complejidad O(n)

    for (u32 i = 0; i < n; i++)
    {   
        u32 vertice_a_colorear;
        if (i < p){
            vertice_a_colorear = Orden[i]; 
        } else {
            vertice_a_colorear = nc(G, Color, Orden, p);
        }
        
        u32 color = colorear(G, vertice_a_colorear, Color); // O(n)
        if (color == NULL_COLOR) {
            return ERROR_CODE;
        }

        if (color_count < color+1) {
            color_count = color+1;
        }
        
        Color[vertice_a_colorear] = color;
    }
    // Complejidad no dinamico O(n^2)
    // Complejidad dinamico O(n^3)
    
    return color_count;
    // Complejidad total: O(n + n^x)
    // con x = 2 si p >= n
    //       = 3 si p aproxima 0
}
