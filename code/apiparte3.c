
#include "apig23.h"
#include "apiparte3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


static u32 colorear (Grafo g, u32 indice_real ,u32* Color){
    u32 grado = Grado(indice_real, g);
    
    if (grado == 0) return 0;

    u32* colores_usados = calloc(NumeroDeVertices(g), sizeof(u32));
    u32 min_color = NULL_COLOR;

    for(u32 i = 0; i < grado; i++){
        u32 color_vecino = Color[IndiceVecino(i,indice_real,g)];

        if (color_vecino != NULL_COLOR) colores_usados[color_vecino] = 1;
    }

    u32 i = 0; 
    while (i < NumeroDeVertices(g) && colores_usados != NULL){
        
        // encuentro el primer hueco 
        if (!colores_usados[i]){
            min_color = i;
            free(colores_usados);
            colores_usados = NULL;
        } else {

        }
        ++i;
    }
    
    return min_color;
    
}

static u32 single_nc(Grafo g, u32 indice_real, u32* Color){
    u32* colores_usados = calloc(NumeroDeVertices(g),sizeof(u32));
    u32 nc = 0;


    for(u32 i = 0; i < NumeroDeVertices(g); i++){
        u32 color_vecino = Color[IndiceVecino(i,indice_real,g)];

        if (color_vecino != NULL_COLOR){
            // si algun vecino nunca habia usado ese color, es un nuevo color => aumenta nc 
            if (colores_usados[color_vecino] == 0) nc ++;
            
            colores_usados[color_vecino] = 1;
        }

    }
    free(colores_usados);
    return nc;
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
}

u32 GreedyDinamico(Grafo G, u32* Orden, u32* Color, u32 p){

    u32 color_count = 0;
    u32 n = NumeroDeVertices(G);
   
    for (u32 i = 0; i < n; i++){
        Color[i] = NULL_COLOR;
    }

    for (u32 i = 0; i < n; i++)
    {   
        u32 vertice_a_colorear;
        if (i < p){
            vertice_a_colorear = Orden[i]; 
        } else {
            vertice_a_colorear = nc(G, Color, Orden, p);
        }
        
        u32 color = colorear(G, vertice_a_colorear, Color);
        if (color == NULL_COLOR) {
            return ERROR_CODE;
        }

        if (color+1  > color_count) {
            color_count = color+1;
        }
        
        Color[vertice_a_colorear] = color;
    }
    
    return color_count;

}





/*
    m(x) = min{Grado(i, G) : Color[i] = x}
    M (x) = max{Grado(i, G) : Color[i] = x}

            0x7fffffff + m(x)   -------------- x % 3 = 2
    E(x) =  0x1fff + M (x)      -------------- x % 3 = 1
            M (x) + m(x)        -------------- x % 3 = 0

    se ponen primero los indices i tq E(Color[i]) es el maximo de los E de ese color

*/

struct data {
    u32 cant_vert;
    u32 *vert;
    u32 m;
    u32 M;
    u32 E;
};


static int compar_FO(const void *elem1, const void *elem2) {
  struct data e1 = *((struct data *)elem1);
  struct data e2= *((struct data *)elem2);
  if (e2.E < e1.E)
        return -1;
    else if (e2.E > e1.E)
        return 1;
    else
        return 0;
}

char FirstOrder(Grafo G, u32* Orden, u32* Color){
    u32 n = NumeroDeVertices(G);
    u32 max_color = 0; 
    u32 cant_color = 0;
    
    
    
    // Cuenta la cantidad de colores que estan siendo usados por Color
    for (u32 i = 0; i < n; i++) {

        if (Color[i] != NULL_COLOR) {
            max_color = max_color < Color[i] ? Color[i] : max_color;
    
        } else {
            return '1';
        }        
    }

    cant_color = max_color +1;
    // Creacion e inicializacion del array data
    // Complejidad O(cantidad de colores usados)
    struct data* data = calloc(cant_color , sizeof(struct data));
    for (u32 i = 0; i < cant_color ; i++) {
        data[i].cant_vert = 0;
        data[i].vert = calloc(n,sizeof(u32));
        data[i].m = MAX_U32;
        data[i].M = 0;
        data[i].E = 0; 
    }


    // Se popula data. 
    // Complejidad O(n)
    for (u32 i = 0; i < n ; i++) {   
        u32 color_i = Color[i];

        if (color_i == NULL_COLOR) {
            return  '1';
        }
        

        u32 grado_i = Grado(i,G);
        data[color_i].vert[data[color_i].cant_vert] = i;
        data[color_i].cant_vert ++;
        data[color_i].M = data[color_i].M < grado_i ? grado_i : data[color_i].M; 
        data[color_i].m = data[color_i].m > grado_i ? grado_i : data[color_i].m;       
        
        
        if (color_i % 3 == 2) {
            data[color_i].E = 0x7fffffff + data[i].m;
        } else if (i % 3 == 1) {
            data[color_i].E = 0x1fff + data[i].M;
        } else {
            data[color_i].E = data[i].M + data[i].m;
        } 
    }
    
    // Ordenamos data segun E con orden descendente 
    // Complejidad O(n log n)
    qsort(data, (size_t)cant_color, sizeof(struct data), compar_FO); 


    // Complejidad O(n)
    u32 orden_index = 0;
    for (u32 color_index = 0; color_index < cant_color; color_index++) { 
        for (u32 v_index = 0; v_index < data[color_index].cant_vert; v_index++) {
            Orden[orden_index] = data[color_index].vert[v_index];
            orden_index ++;
        }
    }
        
    
    
    return '0';
}

char SecondOrder(Grafo G,u32* Orden,u32* Color);
