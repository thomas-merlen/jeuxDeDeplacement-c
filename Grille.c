#include <stdio.h> 
#include <stdlib.h>  
#include "Grille.h"

Grille* Grille_initialiser(int n, int m){
    Grille* g = malloc(sizeof(Grille));
    if (g == NULL){
        return NULL;  // Échec de l'allocation de mémoire
    }

    g->n = n;
    g->m = m;

    // Allocation mémoire pour les lignes
    g->cases = malloc(n * sizeof(ElementGrille*));
    if (g->cases == NULL){
        free(g);  // Nettoyage en cas d'échec
        return NULL;
    }

    // Allocation mémoire pour chaque colonne
    for (int i = 0; i < n; i++){
        g->cases[i] = malloc(m * sizeof(ElementGrille));
        if (g->cases[i] == NULL){
            // Nettoyage en cas d'échec
            for (int j = 0; j < i; j++){
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


void Grille_desallouer(Grille* g){
    if (g != NULL){
        for (int i = 0; i < g->n; i++){
            free(g->cases[i]);
        }
        free(g->cases);
        free(g);
    }
}


void Grille_vider(Grille* g){
    for (int i = 0; i < g->n; i++){
        for (int j = 0; j < g->m; j++){
            g->cases[i][j] = VIDE;
        }
    }
}


void Grille_placer_element(Grille* g, int x, int y, ElementGrille e){
    if (x >= 0 && x < g->n && y >= 0 && y < g->m){
        g->cases[x][y] = e;
    }
}


void Grille_redessiner(const Grille* g){
    for (int i = 0; i < g->n; i++){
        for (int j = 0; j < g->m; j++){
            switch (g->cases[i][j]){
                case VIDE:
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
                case BUT:
                    printf("\33[45m  ");  // carré magenta (but)
                    break;
            }
        }
        printf("\33[1E");
    }
    printf("\n");

    printf("\033[00m"); // Réinitialiser les couleurs à la fin
}

Grille* Grille_charger_fichier(const char* nom_fichier, int* pion_x, int* pion_y){
    FILE* f = fopen(nom_fichier, "r");
    if (!f){
        perror("Erreur ouverture fichier");
        return NULL;
    }

    int n, m;
    fscanf(f, "%d %d\n", &n, &m); // lecture dimensions

    Grille* g = Grille_initialiser(n, m);
    if (!g){
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            char c = fgetc(f);

            switch (c){
                case 'M': g->cases[i][j] = MUR; break;
                case ' ': g->cases[i][j] = VIDE; break;
                case 'Q': g->cases[i][j] = PIEGE; break;
                case 'B': g->cases[i][j] = BUT; break; // si tu utilises un trésor
                case 'P':
                    *pion_x = i;
                    *pion_y = j;
                    g->cases[i][j] = VIDE; // laisse vide pour que le pion y soit placé ensuite
                    break;
                default:
                    g->cases[i][j] = VIDE; // par sécurité
            }
        }
        fgetc(f); // lire le retour à la ligne
    }

    fclose(f);
    return g;
}
