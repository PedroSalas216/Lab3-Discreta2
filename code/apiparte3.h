#ifndef APIParte3_H
#define APIParte3_H

#include "apig23.h"

// Asume que Orden es un array de n elementos que provee un orden de los indices
// Tambien asume que Color apunta a un sector de memoria con al menos n lugares disponibles.

u32 GreedyDinamico(Grafo G, u32* Orden, u32* Color, u32 p);

char FirstOrder(Grafo G, u32* Orden, u32* Color);

char SecondOrder(Grafo G,u32* Orden,u32* Color);

#endif