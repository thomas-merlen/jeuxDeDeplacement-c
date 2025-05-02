all: main_jeu

Fonctions_Jeu.o: Fonctions_Jeu.c Fonctions_Jeu.h Grille.h
	gcc -c -Wall Fonctions_Jeu.c

Grille.o: Grille.c Grille.h
	gcc -c -Wall Grille.c

main_jeu.o: main_jeu.c Fonctions_Jeu.h Grille.h
	gcc -c -Wall main_jeu.c

main_jeu: main_jeu.o Fonctions_Jeu.o Grille.o
	gcc -o main_jeu main_jeu.o Fonctions_Jeu.o Grille.o -lncurses

clean:
	rm -fr *.o main_jeu
