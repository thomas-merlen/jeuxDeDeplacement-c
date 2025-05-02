#include <stdio.h> 
#include <stdlib.h>  
#include "Grille.h"

Grille* Grille_initialiser(int n, int m) {
    Grille* g = malloc(sizeof(Grille));
    if (g == NULL) {
        return NULL;  // Échec de l'allocation de mémoire
    }

    g->n = n;
    g->m = m;

    // Allocation mémoire pour les lignes
    g->cases = malloc(n * sizeof(ElementGrille*));
    if (g->cases == NULL) {
        free(g);  // Nettoyage en cas d'échec
        return NULL;
    }

    // Allocation mémoire pour chaque colonne
    for (int i = 0; i < n; i++) {
        g->cases[i] = malloc(m * sizeof(ElementGrille));
        if (g->cases[i] == NULL) {
            // Nettoyage en cas d'échec
            for (int j = 0; j < i; j++) {
                free(g->cases[j]);
            }
            free(g->cases);
            free(g);
            return NULL;
        }
    }

    // Initialiser toutes les cases à RIEN
    Grille_vider(g);
    return g;
}


void Grille_desallouer(Grille* g) {
    if (g != NULL) {
        for (int i = 0; i < g->n; i++) {
            free(g->cases[i]);
        }
        free(g->cases);
        free(g);
    }
}


void Grille_vider(Grille* g) {
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->m; j++) {
            g->cases[i][j] = RIEN;
        }
    }
}


void Grille_placer_element(Grille* g, int x, int y, ElementGrille e) {
    if (x >= 0 && x < g->n && y >= 0 && y < g->m) {
        g->cases[x][y] = e;
    }
}


void Grille_redessiner(const Grille* g) {
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->m; j++) {
            switch (g->cases[i][j]) {
                case RIEN:
                    printf("\33[42m  ");  // carré vide (vert)
                    break;
                case MUR:
                    printf("\33[41m  ");  // carré rouge (mur)
                    break;
                case PION:
                    printf("\33[44m  ");  // carré bleu (pion)
                    break;
                case PIEGE:
                    printf("\33[43m  ");  // carré jaune (piège)
                    break;
            }
        }
        printf("\n");
    }
}

