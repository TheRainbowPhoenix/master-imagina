#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef struct calc_request {
    char operator;
    int x;
    int y;
} calc_request;

void* calc_plus(void* calc_request); //recupere les message avec l'etiquette '+''
void* calc_minus(void* calc_request);
void* calc_mult(void* calc_request);
void* calc_div(void* calc_request);

int main(int argc, char* argv[]){

    /*
    schema 1:
    setup file de message
    attend requete de client
    calculer resultat requete
    renvoyer resultat requete

    schema 2:
    setup file de message
    boucle
    	attend requete de calculatrice
    	calculer resultat requete
    	renvoyer resultat requete
    fin boucle

    schema 3:
    setup file de message
    setup operator thread
    boucle
    	attend requete de calculatrice
    	calculer resultat requete
    	renvoyer resultat requete
    fin boucle
     */
	return 0;
}
