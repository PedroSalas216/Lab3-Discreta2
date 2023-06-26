
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



// COMIENZA FIRST ORDER
struct data_fo {
    u32 cant_vert;
    u32 *vert;
    u32 m;
    u32 M;
    u32 E;
};

static int compar_FO(const void *elem1, const void *elem2) {
  struct data_fo e1 = *((struct data_fo *)elem1);
  struct data_fo e2 = *((struct data_fo *)elem2);
  if (e2.E < e1.E)
        return -1;
    else if (e2.E > e1.E)
        return 1;
    else
        return 0;
}

char FirstOrder(Grafo G, u32* Orden, u32* Color){
    u32 n = NumeroDeVertices(G);
    u32 r = 0;
    
    for (u32 i = 0; i < n; i++) {
        if (Color[i] != NULL_COLOR) {
            if (r < Color[i] + 1) {
                r = Color[i] + 1;
            }            
        } else {
            return '1';
        }      
    }
    // r = Ji(G)
    // Complejidad 0(n)

    struct data_fo* data = calloc(r , sizeof(struct data_fo));
    for (u32 i = 0; i < r ; i++) {
        data[i].cant_vert = 0;
        data[i].vert = calloc(n,sizeof(u32));
        data[i].m = MAX_U32;
        data[i].M = 0;
        data[i].E = 0; 
    }
    // data[] inicializado
    // Complejidad O(r)

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
    // data[] populado
    // Complejidad O(n)
    
    qsort(data, (size_t)r, sizeof(struct data_fo), compar_FO); 
    // data[] ordenado, descendientemente respecto a data[_].E
    // Complejidad O(r log r)


    u32 orden_index = 0;
    for (u32 color_index = 0; color_index < r; color_index++) { 
        for (u32 v_index = 0; v_index < data[color_index].cant_vert; v_index++) {
            Orden[orden_index] = data[color_index].vert[v_index];
            orden_index ++;
        }
    }
    // traduccion data -> Orden
    // Complejidad O(n)

    for (u32 i = 0; i < r; i++) {
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


// COMIENZA SECOND ORDER
struct data_so {
    u32 cant_vert;
    u32 *vert;
    u32 sum;
};

static int compar_SO(const void *elem1, const void *elem2) {
  struct data_so e1 = *((struct data_so *)elem1);
  struct data_so e2 = *((struct data_so *)elem2);
  if (e1.sum > e2.sum)
        return -1;
    else if (e2.sum > e1.sum)
        return 1;
    else
        return 0;
}

char SecondOrder(Grafo G,u32* Orden,u32* Color){
    u32 n = NumeroDeVertices(G);
    u32 r = 0;
    u32 data_length = 0;

    for (u32 i = 0; i < n; i++) {
        if (Color[i] != NULL_COLOR) {
            if (r < Color[i] + 1) {
                r = Color[i] + 1;
            }            
        } else {
            return '1';
        }
    }
    data_length = r + 1;
    // r = cantidad de colores distintos o X(G)
    // Complejidad O(n)
    

    struct data_so *data = calloc(data_length, sizeof(struct data_so));
    for (u32 i = 0; i < data_length ; i++) {
        data[i].cant_vert = 0;
        data[i].vert = calloc(n, sizeof(u32));
        data[i].sum = 0;
    }
    // data[] inicializado
    // data{0, ... , r-1}   -> vertices con grado mayor a 1, clasificados en r colores
    // data[r]              -> vertices con grado menor o igual a 1
    // Complejidad O(r+1)


    for (u32 i = 0; i < n; i++) {
        u32 color_i = Color[i];
        if (color_i == NULL_COLOR) {
            return  '1';
        }


        u32 grado_i = Grado(i,G);

        if ( grado_i > 1) {
            data[color_i].vert[data[color_i].cant_vert] = i;
            data[color_i].cant_vert ++;
            data[color_i].sum += grado_i;
        } else {
            data[r].vert[data[r].cant_vert] = i;
            data[r].cant_vert ++;
        }
    }
    // data[] inicializado
    // Complejidad O(r+1)
    
    qsort(data, (size_t)data_length, sizeof(struct data_so), compar_SO);
    // data[] ordenado descendientemente respecto a sum
    // Complejidad O(r+1  log r+1)

    u32 orden_index = 0;
   for (u32 color_index = 0; color_index < data_length; color_index++) {
        for (u32 vert_index = 0; vert_index < data[color_index].cant_vert; vert_index++) {
            Orden[orden_index] = data[color_index].vert[vert_index] ;
            orden_index ++;
        }
    }
    // traduccion data -> Orden
    // Complejidad O(n)


    for (u32 i = 0; i < data_length; i++) {
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
