#ifndef _FONCTIONS_JEU_
#define _FONCTIONS_JEU_

#include "Grille.h"
#include "Pion.h"

enum evenement{RIEN, ECHAP, ROUGE, VERT, BLEU, JAUNE};

/* structure pour la liste chaînée des mouvements */
typedef struct Historique{
    EvenementPion mouvement;
    ElementGrille element_remplace; /* position avant le deplacement */
    int x;
    int y;
    struct Historique* suivant;
} Historique;

void Jeu_Partie_A(int option);

void afficher_couleur(enum evenement couleur, int compteur);

int gerer_deplacement_pion(Grille *grille, Pion *pion, enum evenement direction);

void ajouter_historique(Historique** tete, EvenementPion mouvement, ElementGrille element, int x, int y);

int annuler_mouvement(Historique** tete, Pion* pion, Grille* grille);

#endif