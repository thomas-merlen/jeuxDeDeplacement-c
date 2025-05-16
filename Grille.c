#include <stdio.h> 
#include <stdlib.h>  
#include "Grille.h"

/**
 * Initialise une grille de dimensions n x m avec toutes les cases à VIDE.
 * Retourne un pointeur vers la grille nouvellement allouée.
 */
Grille* Grille_initialiser(int n, int m){
    Grille* g = malloc(sizeof(Grille));
    if (g == NULL){
        return NULL;  // Échec de l'allocation de mémoire
    }

    g->n = n;
    g->m = m;

    // Allocation mémoire pour les lignes (tableau de pointeurs)
    g->cases = malloc(n * sizeof(ElementGrille*));
    if (g->cases == NULL){
        free(g);  // Nettoyage si échec
        return NULL;
    }

    // Allocation mémoire pour chaque ligne (tableau de colonnes)
    for (int i = 0; i < n; i++){
        g->cases[i] = malloc(m * sizeof(ElementGrille));
        if (g->cases[i] == NULL){
            // En cas d'échec, libérer les lignes précédemment allouées
            for (int j = 0; j < i; j++){
                free(g->cases[j]);
            }
            free(g->cases);
            free(g);
            return NULL;
        }
    }

    // Initialisation des cases à VIDE
    Grille_vider(g);
    return g;
}

/**
 * Libère la mémoire allouée pour la grille.
 */
void Grille_desallouer(Grille* g){
    if (g != NULL){
        // Libération de chaque ligne
        for (int i = 0; i < g->n; i++){
            free(g->cases[i]);
        }
        free(g->cases);  // Libération du tableau de lignes
        free(g);         // Libération de la structure grille elle-même
    }
}

/**
 * Met toutes les cases de la grille à VIDE.
 */
void Grille_vider(Grille* g){
    for (int i = 0; i < g->n; i++){
        for (int j = 0; j < g->m; j++){
            g->cases[i][j] = VIDE;
        }
    }
}

/**
 * Place un élément à une position (x, y) de la grille, si la position est valide.
 */
void Grille_placer_element(Grille* g, int x, int y, ElementGrille e){
    if (x >= 0 && x < g->n && y >= 0 && y < g->m){
        g->cases[x][y] = e;
    }
}

/**
 * Affiche la grille à l’écran en utilisant des couleurs pour chaque type de case.
 */
void Grille_redessiner(const Grille* g){
    for (int i = 0; i < g->n; i++){
        for (int j = 0; j < g->m; j++){
            switch (g->cases[i][j]){
                case VIDE:
                    printf("\33[42m  ");  // fond vert pour VIDE
                    break;
                case MUR:
                    printf("\33[41m  ");  // fond rouge pour MUR
                    break;
                case PION:
                    printf("\33[44m  ");  // fond bleu pour PION
                    break;
                case PIEGE:
                    printf("\33[43m  ");  // fond jaune pour PIEGE
                    break;
                case BUT:
                    printf("\33[45m  ");  // fond magenta pour BUT
                    break;
            }
        }
        printf("\33[1E");  // Aller à la ligne (équivaut à \n mais plus fluide)
    }
    printf("\n");

    // Réinitialisation des couleurs (important pour ne pas colorer ce qui suit)
    printf("\033[00m");
}


Grille* Grille_charger_fichier(const char* nom_fichier, int* pion_x, int* pion_y){
    FILE* f = fopen(nom_fichier, "r");
    if (!f){
        perror("Erreur ouverture fichier");
        return NULL;
    }

    int n, m;
    fscanf(f, "%d %d\n", &n, &m); // Lecture des dimensions

    // Création d'une grille vide
    Grille* g = Grille_initialiser(n, m);
    if (!g){
        fclose(f);
        return NULL;
    }

    // Remplissage des cases selon le contenu du fichier
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            char c = fgetc(f);

            switch (c){
                case 'M': g->cases[i][j] = MUR; break;
                case ' ': g->cases[i][j] = VIDE; break;
                case 'Q': g->cases[i][j] = PIEGE; break;
                case 'B': g->cases[i][j] = BUT; break;
                case 'P':  // Position initiale du pion
                    *pion_x = i;
                    *pion_y = j;
                    g->cases[i][j] = VIDE; // Laisser la case vide, le pion y sera placé ensuite
                    break;
                default:
                    g->cases[i][j] = VIDE; // Par défaut, éviter les erreurs
            }
        }
        fgetc(f); // Lire le retour à la ligne
    }

    fclose(f);
    return g;
}
