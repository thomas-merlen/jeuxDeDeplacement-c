all: main_jeu

Fonctions_Jeu.o: Fonctions_Jeu.c Fonctions_Jeu.h 
	gcc -c -Wall Fonctions_Jeu.c

main_jeu.o: main_jeu.c Fonctions_Jeu.h
	gcc -c -Wall main_jeu.c

main_jeu: main_jeu.o Fonctions_Jeu.o 
	gcc -o main_jeu main_jeu.o Fonctions_Jeu.o  -lncurses

clean:
	rm -fr *.o main_jeu
