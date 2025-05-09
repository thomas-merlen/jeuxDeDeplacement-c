#include<ncurses.h>
#include "Fonctions_Jeu.h"
#include "Pion.h"
#include "Grille.h"


void afficher_couleur(enum evenement couleur, int compteur){

   printf("\33[2J"); /* Efface tout l'ecran */
   printf("\33[H");  /* Deplace le curseur en position (0,0) */ 
   
   switch (couleur){
      case RIEN:
         printf("%s", "\33[42m  ");  /* Affiche un carré noir */
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
         printf("%s", "\33[42m  ");  /* Affiche un carré noir */
         break;               
   }  

   printf("\033[00m"); /* Declare le fond noir pour la suite */
   printf(" %d\n",compteur);
   printf("\33[1E");      
}
  
void Jeu_Partie_A(int option) {
    enum evenement res; /* Retour des évènements */
    int ch;          /* Récupération de la saisie clavier, valeur # pour fin de boucle */
    int ch_dern = -1;  /* Sauvegarde de la dernière saisie */
    int test_touche; /* Vaut ch ou ch_dern suivant l'option du jeu */
    int compteur; /* Compteur affiché à l'écran */
    int pion_x, pion_y;
    
    /* Charger la grille depuis le fichier */
    Grille* grille = Grille_charger_fichier("grille.txt", &pion_x, &pion_y);
    if (grille == NULL) {
        printf("Erreur lors du chargement de la grille\n");
        return;
    }
    
    /* Créer et placer le pion */
    Pion* pion = Pion_allouer();
    Pion_placer(pion, pion_x, pion_y);
    Grille_placer_element(grille, pion->x, pion->y, PION);

    /* Initialisation de ncurses et du clavier */
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    halfdelay(1);  /* Temps d'exécution max de getch à 1/10ème de seconde */

    ch_dern = -2;    /* Pour permettre le premier affichage */
    compteur = 0;

    do {
        compteur++;
        ch = getch(); /* Si aucune touche utilisée, getch renvoie -1 */

        if (ch_dern == -2) {  /* Affichage une première fois au début */
            printf("\33[2J"); /* Efface tout l'écran */
            printf("\33[H");  /* Déplace le curseur en position (0,0) */
            Grille_redessiner(grille);
            printf("Pour jouer: utiliser les flèches (ESC pour Sortir)\33[1E\33[1E");
            fflush(stdout);
            ch_dern = -1;
        }

        if (ch != -1) {
            if (ch != ch_dern) compteur = 1; /* Remise du compteur à 1 à chaque pression d'une nouvelle touche */
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
                case 27:  // ESC
                    res = ECHAP;
                    break;
                default:
                    res = AUCUN;  // AUCUN doit être défini dans Pion.h
                    break;
            }
            if (res != ECHAP && res != AUCUN) {
                /* Effacer l'ancienne position du pion */
                Grille_placer_element(grille, pion->x, pion->y, VIDE);
                
                /* Déplacer le pion */
                
                Pion_deplacer(pion, res);
                
                /* Vérifier les collisions et mettre à jour la position si nécessaire */
                if (pion->x < 0 || pion->x >= grille->n || pion->y < 0 || pion->y >= grille->m || 
                    grille->cases[pion->x][pion->y] == MUR) {
                    /* Revenir à l'ancienne position si collision avec un mur ou hors limites */
                    pion->x = pion->x_old;
                    pion->y = pion->y_old;
                }
                
                /* Placer le pion à sa nouvelle position */
                Grille_placer_element(grille, pion->x, pion->y, PION);
                
                /* Effacer l'écran et redessiner la grille */
                printf("\33[2J");
                printf("\33[H");
                Grille_redessiner(grille);
                printf("Compteur: %d\n", compteur);
                printf("Utilisez les flèches pour déplacer le pion (ESC pour quitter)\33[1E\33[1E");
                fflush(stdout);
            }
        }
    } while (res != ECHAP);

    /* Nettoyage */
    Grille_desallouer(grille);
    Pion_desallouer(pion);
    
    printf("\n\nAu revoir !\n");
    printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");

    do {
        ch = getch();
    } while(ch == -1);

    endwin(); /* Doit obligatoirement être mis en fin de programme pour remettre le terminal en état */
}