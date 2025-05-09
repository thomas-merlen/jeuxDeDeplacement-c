#include <stdlib.h>
#include <stdio.h>
#include "Pion.h"

Pion* Pion_allouer() {
    Pion* pion = (Pion*)malloc(sizeof(Pion));
    if (pion != NULL) {
        pion->x = 0;
        pion->y = 0;
        pion->x_old = 0;
        pion->y_old = 0;
    }
    return pion;
}

void Pion_desallouer(Pion* pion) {
    if (pion != NULL) {
        free(pion);
    }
}

void Pion_placer(Pion* pion, int x, int y) {
    if (pion != NULL) {
        pion->x_old = pion->x;
        pion->y_old = pion->y;
        pion->x = x;
        pion->y = y;
    }
}