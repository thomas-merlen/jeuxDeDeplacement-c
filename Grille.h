#ifndef GRILLE_H
#define GRILLE_H

/* taille maximale grille */
#define MAX_N 100
#define MAX_M 100

typedef enum{
    VIDE,   /* case vide */
    MUR,    
    PION,   /* joueur */
    PIEGE,
    BUT
} ElementGrille;

/* grille du jeu */
typedef struct{
    int n; /* lignes */
    int m; /* colonnes */
    ElementGrille** cases;
} Grille;

/* initialise la grille */
Grille* Grille_initialiser(int n, int m);

/* désalloue la grille et son contenu */
void Grille_desallouer(Grille* g);

/* rempli la grille d'element VIDE */
void Grille_vider(Grille* g);

/* dessine à l’écran ce qui est codé dans le tableau de la Grille */
void Grille_redessiner(const Grille* g);

/* placer un element sur la grille */
void Grille_placer_element(Grille* g, int x, int y, ElementGrille e);

Grille* Grille_charger_fichier(const char* nom_fichier, int* pion_x, int* pion_y);

#endif
