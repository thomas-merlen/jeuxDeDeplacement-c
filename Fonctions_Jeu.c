#include<ncurses.h>
#include "Fonctions_Jeu.h"


void afficher_couleur(enum evenement couleur, int compteur){

   printf("\33[2J"); /* Efface tout l'ecran */
   printf("\33[H");  /* Deplace le curseur en position (0,0) */ 
   
   switch (couleur){
      case RIEN:
         printf("%s", "\33[42m  ");  /* Affiche un carré noir */
         break;      
      case VERT:
         printf("%s", "\33[42m  ");  /* Affiche un carré vert */
         break;
      case ROUGE:
         printf("%s", "\33[41m  ");  /* Affiche un carré rouge */
         break;
      case BLEU:
         printf("%s", "\33[44m  ");  /* Affiche un carré bleu */
         break;
      case JAUNE:
         printf("%s", "\33[43m  ");  /* Affiche un carré jaune */
         break;   
      default:
         printf("%s", "\33[42m  ");  /* Affiche un carré noir */
         break;               
   }  

   printf("\033[00m"); /* Declare le fond noir pour la suite */
   printf(" %d\n",compteur);
   printf("\33[1E");      
}
  
void Jeu_Partie_A(int option){

  enum evenement res; /* Retour des évènements  */

  int ch;          /* Récupération de la saisie clavie, valeur # pour fin de boucle */
  int ch_dern=-1;  /* Sauvegarde de la dernière saisie */
  int test_touche; /* Vaut ch ou ch_dern suivant l'option du jeu */
  
  int compteur; /* Compteur affiché à l'écran */
  
  /* Initialisation de ncurses et du clavier(4 lignes)*/
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  halfdelay(1);  /* Temps d'exécution max de getch à 1/10ème de seconde */

  ch_dern=-2;    /* Pour permettre le premier affichage */
  
  compteur=0;
  
  do{
  
    compteur++;

    ch = getch(); /* Si aucune touche utilisée, getch renvoie -1 */   

 
    if (ch_dern==-2){  /* Affichage une première fois au début */
      afficher_couleur(ROUGE, compteur);
      printf("Pour jouer: utiliser les flèches (ESC pour Sortir)\33[1E\33[1E");
      fflush(stdout);  
      ch_dern=-1; 
    }

    if (ch!=-1){
      if (ch!=ch_dern) compteur=1; /* Remise du compteur à 1 à chaque pression d'une nouvelle touche */
      ch_dern=ch;
    }
        
    if ( (ch!=-1) || ( (ch==-1) && (option==2) ) ){  

     if (option==1) test_touche=ch;      /* Si on teste ch, le pion est en attente du joueur à chaque instant */
               else test_touche=ch_dern; /* Si on teste ch_dern, le pion continue à avancer */
            
     switch(test_touche) {  
                  
        case KEY_UP:   /* Ces constantes sont dans ncurses pour correspondre aux codes de touches */
           printf("La derniere touche utilisée est: Up Arrow\33[1E");          
           res=ROUGE;
           break;
        case KEY_DOWN: 
           printf("La derniere touche utilisée est: Down Arrow\33[1E");       
           res=VERT;
           break;
        case KEY_LEFT: 
           printf("La derniere touche utilisée est: Left Arrow\33[1E");
           res=BLEU;
           break; 
        case KEY_RIGHT: 
           printf("La derniere touche utilisée est: Right Arrow\33[1E");
           res=JAUNE;
           break;
        case 27:  /* Code de la touche ESC-Echap */
           res=ECHAP;
           printf("La derniere touche utilisée est: ESC\33[1E");
           break;
        default:
           printf("La derniere touche utilisee est: %c\33[1E", test_touche);
           break;
           
       }
      
      if (res!=ECHAP){
         afficher_couleur(res, compteur);
         
         printf("Pour changer la couleur du carré: utiliser les flèches (ESC pour Sortir)\33[1E\33[1E");
         
      }
      
      fflush(stdout);  /* Force l'affichage complet des commandes précédentes */
     
     }

  } while (res!=ECHAP);

     
  printf("\n\nAu revoir !\n");        
      
  printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");     
   
   
  do{
      ch = getch();
  } while(ch==-1);
  

  endwin(); /* Doit obligatoirement etre mis en fin de programme pour remettre le terminal en etat */  
  
 }
