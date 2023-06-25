#include "apiparte3.h"
#include "stdio.h"

#define NULL_COLOR __UINT32_MAX__

bool checkear_coloreo(Grafo G, u32 *Color) {
    // O(m)
    for (u32 index = 0; index < NumeroDeVertices(G); ++index) {
        if (Color[index] == NULL_COLOR) { // si hay un vertice que no tiene color esta mal
            return false;
        }
        for (u32 vecino = 0; vecino < Grado(index, G); ++vecino) {
            if (Color[index] == Color[IndiceVecino(vecino, index, G)]) {
                return false;   
            }
        }
    }
    return true;
}



int main() {
    Grafo g = ConstruirGrafo();
    // DestruirGrafo(g);
    printf("se termino de cargar\n");

    u32 n = NumeroDeVertices(g);
    u32 *color = calloc(n, sizeof(u32));
    u32 *orden = calloc(n, sizeof(u32));

    for (u32 i = 0; i < n; i++) {
        color[i] = NULL_COLOR;
        orden[i] = i;
    }
    

    printf("Fin inicializaciones \n");
    u32 ji = GreedyDinamico(g,orden,color,n +1 );
        

    printf("Fin greedy \n");
    if (checkear_coloreo(g,color)){
        printf("Coloreo Propio: X(G) ~ %u\n", ji);
    }else{
        printf("Coloreo No propio \n");
    }
    

    DestruirGrafo(g);
    return 0;
}