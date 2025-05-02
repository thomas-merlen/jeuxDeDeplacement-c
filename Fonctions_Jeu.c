#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "Fonctions_Jeu.h"
#include "Grille.h"

void afficher_couleur(enum evenement couleur, int compteur) {
    printf("\33[2J"); /* Efface tout l'écran */
    printf("\33[H");  /* Deplace le curseur en position (0,0) */ 
    
    switch (couleur) {
        case RIEN:
            printf("%s", "\33[42m  ");  /* Affiche un carré vert (VIDE) */
            break;      
        case VERT:
            printf("%s", "\33[42m  ");  /* Affiche un carré vert */
            break;
        case ROUGE:
            printf("%s", "\33[41m  ");  /* Affiche un carré rouge */
            break;
        case BLEU:
            printf("%s", "\33[44m  ");  /* Affiche un carré bleu */
            break;
        case JAUNE:
            printf("%s", "\33[43m  ");  /* Affiche un carré jaune */
            break;   
        default:
            printf("%s", "\33[42m  ");  /* Affiche un carré vert par défaut */
            break;               
    }  

    printf("\033[00m"); /* Réinitialise les couleurs */
    printf(" %d\n", compteur);
    printf("\33[1E");      
}

void Jeu_Partie_A(int option) {
    enum evenement res;
    int ch, ch_dern = -1, test_touche, compteur;
    
    /* ===== TEST DE LA GRILLE ===== */
    Grille* grille_test = Grille_initialiser(5, 5);
    if (grille_test == NULL) {
        printf("Erreur: impossible d'initialiser la grille\n");
        return;
    }

    // Placement d'éléments de test
    Grille_placer_element(grille_test, 0, 0, MUR);    // Coin supérieur gauche (rouge)
    Grille_placer_element(grille_test, 2, 2, PION);   // Centre (bleu)
    Grille_placer_element(grille_test, 4, 4, PIEGE);  // Coin inférieur droit (jaune)
    
    printf("\33[2J\33[H");  // Efface l'écran et place le curseur en haut
    printf("=== TEST DE LA GRILLE ===\n");
    Grille_redessiner(grille_test);
    printf("\033[00m");     // Réinitialise les couleurs
    printf("\nAppuyez sur une touche pour commencer le jeu...\n");
    fflush(stdout);
    
    while (getch() == -1);  // Attente d'une touche
    Grille_desallouer(grille_test);
    /* ===== FIN DU TEST ===== */

    /* Initialisation de ncurses */
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
            afficher_couleur(ROUGE, compteur);
            printf("Pour jouer: utiliser les flèches (ESC pour Sortir)\33[1E\33[1E");
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
                    printf("Up Arrow\33[1E");          
                    res = ROUGE;
                    break;
                case KEY_DOWN:
                    printf("Down Arrow\33[1E");       
                    res = VERT;
                    break;
                case KEY_LEFT:
                    printf("Left Arrow\33[1E");
                    res = BLEU;
                    break;
                case KEY_RIGHT:
                    printf("Right Arrow\33[1E");
                    res = JAUNE;
                    break;
                case 27:
                    res = ECHAP;
                    printf("ESC\33[1E");
                    break;
                default:
                    printf("%c\33[1E", test_touche);
                    break;
            }
          
            if (res != ECHAP) {
                afficher_couleur(res, compteur);
                printf("Pour changer la couleur du carré: utiliser les flèches (ESC pour Sortir)\33[1E\33[1E");
                fflush(stdout);
            }
        }
    } while (res != ECHAP);

    printf("\n\nAu revoir !\n");        
    printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");     
    
    do {
        ch = getch();
    } while(ch == -1);
    
    endwin();
}