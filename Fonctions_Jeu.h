#ifndef _FONCTIONS_JEU_
#define _FONCTIONS_JEU_

#include "Grille.h"
#include "Pion.h"

enum evenement {RIEN, ECHAP, ROUGE, VERT, BLEU, JAUNE};

// Structure pour la liste chaînée des mouvements
typedef struct Historique {
    EvenementPion mouvement;
    ElementGrille element_remplace; // Ce qui était à cette position avant le déplacement
    int x;
    int y;
    struct Historique* suivant;
} Historique;

/* Coeur du jeu contenant la boucle événementielle */
void Jeu_Partie_A(int option);

/* Afficher un carré de couleur*/
void afficher_couleur(enum evenement couleur, int compteur);

int gerer_deplacement_pion(Grille *grille, Pion *pion, enum evenement direction);

#endif