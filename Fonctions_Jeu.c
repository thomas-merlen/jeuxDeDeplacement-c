#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "Fonctions_Jeu.h"
#include "Pion.h"
#include "Grille.h"

/* fonction principale */
void Jeu_Partie_A(int option){
    enum evenement res;      /* action du joueur */
    int ch;                  /* touche pressée */
    int ch_dern = -1;        /* dernière touche pressée */
    int test_touche;         
    int compteur;            
    int pion_x, pion_y;      /* position initiale */
    Historique* historique = NULL; /* liste historique de mouvement */
    
    /* chargement grille */
    Grille* grille = Grille_charger_fichier("grille.txt", &pion_x, &pion_y);
    if (grille == NULL){
        printf("Erreur lors du chargement de la grille\n");
        return;
    }
    
    /* init pion dans la grille */
    Pion* pion = Pion_allouer();
    Pion_placer(pion, pion_x, pion_y);
    Grille_placer_element(grille, pion->x, pion->y, PION);

    /* init ncurses */
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    halfdelay(1); 

    ch_dern = -2;
    compteur = 0;

    do{
        compteur++;
        ch = getch(); /* lire la touche presser */

        /* init affichage */
        if (ch_dern == -2){
            printf("\33[2J"); /* efface l'écran */
            printf("\33[H");  /* curseur en haut à gauche */
            Grille_redessiner(grille);
            printf("Pour jouer: utiliser les flèches (ESC pour Sortir, DEL pour UNDO)\33[1E\33[1E");
            fflush(stdout);
            ch_dern = -1;
        }

        /* dernier caractère lu */
        if (ch != -1){
            if (ch != ch_dern) compteur = 1;
            ch_dern = ch;
        }

        /* traitement de la touche */
        if ((ch != -1) || ((ch == -1) && (option == 2))){
            test_touche = (option == 1) ? ch : ch_dern;

            /* association touche action */
            switch(test_touche){
                case KEY_UP: res = HAUT; break;
                case KEY_DOWN: res = BAS; break;
                case KEY_LEFT: res = GAUCHE; break;
                case KEY_RIGHT: res = DROITE; break;
                case KEY_DC:  /* suppr */
                case 'z':     
                    res = UNDO;
                    break;
                case 27:      /* echap */
                    res = ECHAP;
                    break;
                default:
                    res = AUCUN;
                    break;
            }

            /* annulation du mouvement */
            if ((int)res == (int)UNDO){
                if (annuler_mouvement(&historique, pion, grille)){
                    /* afficher grille apres UNDO */
                    printf("\33[2J");
                    printf("\33[H");
                    Grille_redessiner(grille);
                    printf("UNDO effectué! Compteur: %d\n", compteur);
                    printf("Utilisez les flèches pour déplacer le pion (ESC pour quitter, DEL pour UNDO)\33[1E\33[1E");
                    fflush(stdout);
                }
            }
            /* si mouvement autre que UNDO ou Échap */
            else if ((int)res != (int)ECHAP && (int)res != (int)AUCUN){
                /* sauvegarde de l'élément actuel pour pouvoir revenir en arrière */
                ElementGrille element_avant = grille->cases[pion->x][pion->y];
                
                /* effacement de l'ancienne position du pion */
                Grille_placer_element(grille, pion->x, pion->y, VIDE);
                
                Pion_deplacer(pion, res);
                
                /* si collision avec un mur ou sortie de la grille, revenir en arrière */
                if (pion->x < 0 || pion->x >= grille->n || pion->y < 0 || pion->y >= grille->m || 
                    grille->cases[pion->x][pion->y] == MUR){
                    pion->x = pion->x_old;
                    pion->y = pion->y_old;
                }
                else{
                    /* ajouter à l'historique uniquement si le déplacement est valide */
                    ajouter_historique(&historique, res, element_avant, pion->x_old, pion->y_old);
                }

                /* si le pion atteint un piège ou le but */
                if (grille->cases[pion->x][pion->y] == PIEGE || 
                    grille->cases[pion->x][pion->y] == BUT){
                    fflush(stdout);
                    res = ECHAP;
                }

                Grille_placer_element(grille, pion->x, pion->y, PION);
                
                printf("\33[2J");
                printf("\33[H");
                Grille_redessiner(grille);
                printf("Compteur: %d\n", compteur);
                printf("Utilisez les flèches pour déplacer le pion (ESC pour quitter, DEL pour UNDO)\33[1E\33[1E");
                fflush(stdout);
            }
        }
    } while (res != ECHAP); /* boucle jusqu'à ce que le joueur appuie sur Échap */

    /* libération de la mémoire de l'historique */
    while (historique != NULL){
        Historique* suivant = historique->suivant;
        free(historique);
        historique = suivant;
    }
    
    /* libération de la mémoire */
    Grille_desallouer(grille);
    Pion_desallouer(pion);
    
    /* fin du jeu */
    printf("\n\nAu revoir !\n");
    printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");

    do{
        ch = getch(); 
    } while(ch == -1);

    endwin(); 
}


void ajouter_historique(Historique** tete, EvenementPion mouvement, ElementGrille element, int x, int y){
    Historique* nouveau = malloc(sizeof(Historique));
    if (nouveau == NULL) return;
    
    nouveau->mouvement = mouvement;
    nouveau->element_remplace = element;
    nouveau->x = x;
    nouveau->y = y;
    nouveau->suivant = *tete;
    *tete = nouveau;
}

int annuler_mouvement(Historique** tete, Pion* pion, Grille* grille){
    if (*tete == NULL) return 0; /* rien à annuler */
    
    Historique* a_supprimer = *tete;
    *tete = a_supprimer->suivant;

    /* effacer la position actuelle du pion */
    Grille_placer_element(grille, pion->x, pion->y, VIDE);

    /* restaurer l'ancienne position du pion */
    pion->x_old = pion->x;
    pion->y_old = pion->y;
    pion->x = a_supprimer->x;
    pion->y = a_supprimer->y;
    
    /* restaurer l'élément présent avant le déplacement */
    Grille_placer_element(grille, pion->x, pion->y, a_supprimer->element_remplace);
    
    free(a_supprimer);
    return 1;
}
