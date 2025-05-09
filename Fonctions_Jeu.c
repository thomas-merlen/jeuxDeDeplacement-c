#include<ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "Fonctions_Jeu.h"
#include "Pion.h"
#include "Grille.h"

// Fonction pour ajouter un mouvement à l'historique
void ajouter_historique(Historique** tete, EvenementPion mouvement, ElementGrille element, int x, int y) {
    Historique* nouveau = malloc(sizeof(Historique));
    if (nouveau == NULL) return;
    
    nouveau->mouvement = mouvement;
    nouveau->element_remplace = element;
    nouveau->x = x;
    nouveau->y = y;
    nouveau->suivant = *tete;
    *tete = nouveau;
}

// Fonction pour annuler le dernier mouvement
int annuler_mouvement(Historique** tete, Pion* pion, Grille* grille) {
    if (*tete == NULL) return 0; // Rien à annuler
    
    Historique* a_supprimer = *tete;
    *tete = a_supprimer->suivant;

    // Effacer la position actuelle du pion (avant l'annulation)
    Grille_placer_element(grille, pion->x, pion->y, VIDE);

    // Restaurer l'ancienne position du pion
    pion->x_old = pion->x;
    pion->y_old = pion->y;
    pion->x = a_supprimer->x;
    pion->y = a_supprimer->y;
    
    // Restaurer l'élément qui était à cette position
    Grille_placer_element(grille, pion->x, pion->y, a_supprimer->element_remplace);
    
    free(a_supprimer);
    return 1;
}


void Jeu_Partie_A(int option) {
    enum evenement res;
    int ch;
    int ch_dern = -1;
    int test_touche;
    int compteur;
    int pion_x, pion_y;
    Historique* historique = NULL; // Initialisation de l'historique
    
    Grille* grille = Grille_charger_fichier("grille.txt", &pion_x, &pion_y);
    if (grille == NULL) {
        printf("Erreur lors du chargement de la grille\n");
        return;
    }
    
    Pion* pion = Pion_allouer();
    Pion_placer(pion, pion_x, pion_y);
    Grille_placer_element(grille, pion->x, pion->y, PION);

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    halfdelay(1);

    ch_dern = -2;
    compteur = 0;

    do {
        compteur++;
        ch = getch();

        if (ch_dern == -2) {
            printf("\33[2J");
            printf("\33[H");
            Grille_redessiner(grille);
            printf("Pour jouer: utiliser les flèches (ESC pour Sortir, DEL pour UNDO)\33[1E\33[1E");
            fflush(stdout);
            ch_dern = -1;
        }

        if (ch != -1) {
            if (ch != ch_dern) compteur = 1;
            ch_dern = ch;
        }

        if ((ch != -1) || ((ch == -1) && (option == 2))) {
            if (option == 1) test_touche = ch;
            else test_touche = ch_dern;

            switch(test_touche) {
                case KEY_UP:
                    res = HAUT; 
                    break;
                case KEY_DOWN:
                    res = BAS;
                    break;
                case KEY_LEFT:
                    res = GAUCHE;
                    break;
                case KEY_RIGHT:
                    res = DROITE;
                    break;
                case KEY_DC:  // Touche DEL/SUPPR
                case 'z':     // Touche Z (alternative)
                    res = UNDO;
                    break;
                case 27:  // ESC
                    res = ECHAP;
                    break;
                default:
                    res = AUCUN;
                    break;
            }

            if (res == UNDO) {
                if (annuler_mouvement(&historique, pion, grille)) {
                    // Redessiner après l'annulation
                    printf("\33[2J");
                    printf("\33[H");
                    Grille_redessiner(grille);
                    printf("UNDO effectué! Compteur: %d\n", compteur);
                    printf("Utilisez les flèches pour déplacer le pion (ESC pour quitter, DEL pour UNDO)\33[1E\33[1E");
                    fflush(stdout);
                }
            }
            else if (res != ECHAP && res != AUCUN) {
                // Sauvegarder l'élément actuel avant de déplacer le pion
                ElementGrille element_avant = grille->cases[pion->x][pion->y];
                
                // Effacer l'ancienne position du pion
                Grille_placer_element(grille, pion->x, pion->y, VIDE);
                
                // Déplacer le pion
                Pion_deplacer(pion, res);
                
                // Vérifier les collisions
                if (pion->x < 0 || pion->x >= grille->n || pion->y < 0 || pion->y >= grille->m || 
                    grille->cases[pion->x][pion->y] == MUR) {
                    // Revenir à l'ancienne position si collision
                    pion->x = pion->x_old;
                    pion->y = pion->y_old;
                }
                else {
                    // Ajouter le mouvement à l'historique seulement si valide
                    ajouter_historique(&historique, res, element_avant, pion->x_old, pion->y_old);
                }

                // Vérifier les conditions de fin de jeu
                if (grille->cases[pion->x][pion->y] == PIEGE || 
                    grille->cases[pion->x][pion->y] == BUT) {
                    fflush(stdout);
                    res = ECHAP;
                }

                // Placer le pion à sa nouvelle position
                Grille_placer_element(grille, pion->x, pion->y, PION);
                
                // Redessiner
                printf("\33[2J");
                printf("\33[H");
                Grille_redessiner(grille);
                printf("Compteur: %d\n", compteur);
                printf("Utilisez les flèches pour déplacer le pion (ESC pour quitter, DEL pour UNDO)\33[1E\33[1E");
                fflush(stdout);
            }
        }
    } while (res != ECHAP);

    // Nettoyage de l'historique
    while (historique != NULL) {
        Historique* suivant = historique->suivant;
        free(historique);
        historique = suivant;
    }
    
    Grille_desallouer(grille);
    Pion_desallouer(pion);
    
    printf("\n\nAu revoir !\n");
    printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");

    do {
        ch = getch();
    } while(ch == -1);

    endwin();
}