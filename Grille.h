#ifndef GRILLE_H
#define GRILLE_H

/*taille maximale grille*/
#define MAX_N 100
#define MAX_M 100

typedef enum {
    RIEN,   /*case vide*/
    MUR,    
    PION,   /*joueur*/
    PIEGE   
} ElementGrille;

/*grille du jeu*/
typedef struct {
    int n; /*lignes*/
    int m; /*colonnes*/
    ElementGrille cases[MAX_N][MAX_M];
} Grille;

#endif
