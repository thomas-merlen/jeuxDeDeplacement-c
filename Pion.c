#include <stdlib.h>
#include <stdio.h>
#include "Pion.h"

/**
 * Alloue dynamiquement un pion et initialise ses coordonnées à 0.
 * Retourne un pointeur vers le pion créé, ou NULL en cas d'échec.
 */
Pion* Pion_allouer(){
    Pion* pion = (Pion*)malloc(sizeof(Pion));
    if (pion != NULL){
        pion->x = 0;        // Position actuelle en x
        pion->y = 0;        // Position actuelle en y
        pion->x_old = 0;    // Ancienne position en x
        pion->y_old = 0;    // Ancienne position en y
    }
    return pion;
}

/**
 * Libère la mémoire associée à un pion.
 */
void Pion_desallouer(Pion* pion){
    if (pion != NULL){
        free(pion);
    }
}

/**
 * Place le pion à une nouvelle position (x, y).
 * Sauvegarde également la position précédente pour pouvoir revenir en arrière.
 */
void Pion_placer(Pion* pion, int x, int y){
    if (pion != NULL){
        pion->x_old = pion->x;  // Sauvegarde ancienne position x
        pion->y_old = pion->y;  // Sauvegarde ancienne position y
        pion->x = x;            // Mise à jour de la nouvelle position x
        pion->y = y;            // Mise à jour de la nouvelle position y
    }
}

/**
 * Déplace le pion dans une direction donnée selon un événement (HAUT, BAS, GAUCHE, DROITE).
 * Sauvegarde la position actuelle avant de la modifier.
 * L'événement UNDO est ignoré ici (géré ailleurs).
 */
void Pion_deplacer(Pion* pion, EvenementPion e){
    if (pion == NULL) return;

    // Sauvegarder les anciennes positions avant le déplacement
    pion->x_old = pion->x;
    pion->y_old = pion->y;

    switch (e){
        case HAUT:
            pion->x -= 1;  // Déplacement vers le haut (ligne précédente)
            break;
        case BAS:
            pion->x += 1;  // Déplacement vers le bas (ligne suivante)
            break;
        case GAUCHE:
            pion->y -= 1;  // Déplacement à gauche (colonne précédente)
            break;
        case DROITE:
            pion->y += 1;  // Déplacement à droite (colonne suivante)
            break;
        case UNDO:
            // Ne rien faire ici, sera géré ailleurs
            break;
        default:
            break;
    }
}
