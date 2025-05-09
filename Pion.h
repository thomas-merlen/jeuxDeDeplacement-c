#ifndef PION_H
#define PION_H

typedef struct {
    int x;       /*position en x*/
    int y;       /*position en y*/
    int x_old;   /*ancienne position en x*/
    int y_old;   /*ancienne position en y*/
} Pion;

typedef enum {
    AUCUN,
    HAUT,
    BAS,
    GAUCHE,
    DROITE
} EvenementPion;

/*alloue un pion*/
Pion* Pion_allouer();

/*desalloue un pion*/
void Pion_desallouer(Pion* pion);

/*placer un pion*/
void Pion_placer(Pion* pion, int x, int y);

void Pion_deplacer(Pion* pion, EvenementPion e);

#endif


