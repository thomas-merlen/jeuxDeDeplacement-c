#include <stdio.h> 
#include <stdlib.h>  
#include "Grille.h"

Grille* Grille_initialiser(int n, int m) {
    if (n <= 0 || m <= 0 || n > MAX_N || m > MAX_M) {
        fprintf(stderr, "Dimensions invalides\n");
        return NULL;
    }

    Grille* g = malloc(sizeof(Grille));
    if (!g) {
        perror("Erreur d'allocation de la grille");
        return NULL;
    }

    g->n = n;
    g->m = m;
    Grille_vider(g);
    return g;
}

void Grille_desallouer(Grille* g) {
    if (g) {
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

// Affichage avec séquences ANSI
void Grille_redessiner(const Grille* g) {
    printf("\033[H"); // Retour en haut à gauche
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->m; j++) {
            switch (g->cases[i][j]) {
                case RIEN:
                    printf("\033[40m  "); // fond noir
                    break;
                case MUR:
                    printf("\033[47m  "); // fond blanc
                    break;
                case PION:
                    printf("\033[42m  "); // fond vert
                    break;
                case PIEGE:
                    printf("\033[41m  "); // fond rouge
                    break;
                default:
                    printf("\033[40m  ");
            }
        }
        printf("\033[0m\n");
    }
}
