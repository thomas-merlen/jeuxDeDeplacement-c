#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Fonctions_Jeu.h"


int main(int argv, char **argc){

  if (argv!=2){
     printf("Usage: %s option\n",argc[0]);
     printf("   où option vaut 1: si jeu pas à pas / 2: si jeu en continue\n");
     exit(2);
  }

  srand(time(NULL));

  Jeu_Partie_A(atoi(argc[1]));
  
  return EXIT_SUCCESS;
}


