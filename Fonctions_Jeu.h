#ifndef _FONCTIONS_JEU_
#define _FONCTIONS_JEU_

enum evenement {RIEN, ECHAP, ROUGE, VERT, BLEU, JAUNE};

/* Coeur du jeu contenant la boucle événementielle 
   L'option 1 attend chaque pression d'une touche pour effectuer une action
   L'option 2 répète l'action corresponde à la pression de la dernière touche 
*/
void Jeu_Partie_A(int option);

/* Afficher un carré de couleur*/
void afficher_couleur(enum evenement couleur, int compteur);

#endif
