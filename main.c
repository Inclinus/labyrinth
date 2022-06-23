#include <stdio.h>
#include <stdlib.h>
#include "level.h"

int main(int argc, char **argv){

    // Start
    printf("Bienvenue sur le Labyrinthe ! Que pouvons nous faire pour vous ?\n\n");

    // Labyrinth game gameloop
    int keepGoing;
    // Menu input
    int menu;
    // Global gameloop
    int quit = 0;

    while (quit!=1){
        menu = -1;
        while (menu < 1 || menu > 3){
            printf("Choisissez :\n1 - Jeu du Labyrinthe\n2 - Credits\n3 - Quitter\n\n");
            scanf("%d",&menu);
        }
        switch (menu) {
            case 1:
                keepGoing = 1;
                while (keepGoing){
                    int size;
                    do{
                        printf("\nChoisissez une taille (entre 10 et 40) :\n");
                        scanf("%d",&size);
                    } while (size < 10 || size > 40);

                    char bombs = 'u'; // initialize with stuff
                    while (bombs!='o' && bombs!='n'){
                        printf("\nSouhaitez-vous des bombes sur le labyrinthe ? (o/n) :\n");
                        scanf(" %c",&bombs);
                    }

                    if(!levelMain(argc,argv,size,bombs=='o')){

                        char answer = 'u'; // initialize with stuff
                        while (answer!='o' && answer!='n'){
                            printf("Voulez-vous rejouer ? (o/n) :\n");
                            scanf(" %c",&answer);
                        }

                        if(answer=='n') {
                            printf("\nC'était un plaisir de vous avoir avec nous, bonne fin de journée !\n");
                            printf("\n###############\nDeveloped by :\nAlexandre COMET\nNoam DE MASURE\nThibaut LULINSKI\n\nAll rights reserved.\n###############\n\n");
                            keepGoing = 0;
                        }
                    }
                }
                break;
            case 2:
                printf("\n###############\n\nDeveloped by :\n\nAlexandre COMET\nNoam DE MASURE\nThibaut LULINSKI\n\n\nAll rights reserved.\n\n###############\n\n");
                break;
            case 3:
                quit = 1;
                break;
            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}
