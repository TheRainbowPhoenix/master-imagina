#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "etape.h"

void print_tab(int n){
	for (int i = 0; i < n; ++i){
		printf("\t");
	}
}

struct sNverrou{
  int current_thread_number;
  int max_thread_number;
  pthread_mutex_t verrou;
  pthread_cond_t cond;
};

typedef struct sNverrou n_verrou;

struct sParams {
   int indice;
   n_verrou* nvTab;
};

typedef struct sParams params;

// LE VERROU SUIVANT EST UTILISE UNIQUEMENT POUR LA SORTIE STANDARD. 
// NE PAS LE REUTILISER POUR AUTRE FONCTION QUE L'AFFICHAGE. VOIR UTILISATION PLUS LOIN.
pthread_mutex_t vStdOut= PTHREAD_MUTEX_INITIALIZER;

// initialisation des champs de la structure n_verrou
int n_verrou_init(n_verrou * v, int k){
  v->max_thread_number = k;
  v->current_thread_number = 0;
  pthread_cond_init(&v->cond, NULL);
  return pthread_mutex_init(&v->verrou, NULL);
}

// bloque si le nombre maximum de threads autorisés à passer en section
// critique est atteint, sinon, permet l'accès à la section critique
int n_verrou_lock(n_verrou * v){

  pthread_mutex_lock(&v->verrou);

  v->current_thread_number++;

  while(v->current_thread_number > v->max_thread_number){
    pthread_cond_wait(&v->cond, &v->verrou);
  }

  return pthread_mutex_unlock(&v->verrou);
}

int im_verrou_lock(im_verrou * v){

  pthread_mutex_lock(&v->verrou);

  v->nb_thread_waiting++;

  if(v->nb_thread_waiting < v->init_thread_number) {
    do {
      pthread_cond_wait(&v->cond, &v->verrou);
    } while(v->current_thread_inside > v->max_thread_number){
  }

  v->nb_thread_inside++;
  
  pthread_cond_signal(&v->cond);

  return pthread_mutex_unlock(&v->verrou);
}

// décrémente le nombre de thread en section critique et reveille
// éventuellement un thread en attente d'entrer en section critique
int n_verrou_unlock(n_verrou * v){

  pthread_mutex_lock(&v->verrou);
  v->nb_thread_waiting--;
  v->nb_thread_inside--;
  pthread_cond_signal(&v->cond);
  
  return pthread_mutex_unlock(&v->verrou);
}

int n_verrou_destroy(n_verrou * v){
  v->max_thread_number = 0;
  v->current_thread_number = 0;
  
  return pthread_cond_destroy(&v->cond);
}


// Fonction exécutée par chaque thread (joueur). NE PAS MODIFIER CETTE FONCTION !
void * jouer(void * p){

  params * param = (params*)p;
  
  // LE VERROU vStdOut EST UTILISE UNIQUEMENT POUR L'AFFICHAGE (ACCES A LA SORTIE STANDARD).
  pthread_mutex_lock(&vStdOut); printf("Joueur %d: je suis pret\n", param -> indice);  pthread_mutex_unlock(&vStdOut);
  
  // Etape 1 :
  
  // je demande à passer : je passe s'il y a moins de
  // maxNbJoueursEtape1 joueurs présents dans cette étape, sinon
  // j'attends que ça soit le cas.
  if(n_verrou_lock(&(param -> nvTab[0])) != 0) {
    perror("probleme n_verrou lock");
    free(param);
    pthread_exit(NULL);
  }
  
  // rappel : vStdOut n'est utilisé que pour les affichages et n'affecte en rien le déroulement du jeu.
  pthread_mutex_lock(&vStdOut); printf("Joueur %d: debut etape 1\n", param -> indice); pthread_mutex_unlock(&vStdOut);
  
  etape();
  
  pthread_mutex_lock(&vStdOut); printf("Joueur %d: fin etape 1\n", param -> indice); pthread_mutex_unlock(&vStdOut);
  
  // je viens de finir une étape, je dois le signaler.
  if(n_verrou_unlock(&(param->nvTab[0])) != 0) {
    perror("probleme n_verrou unlock");
    free(param);
    pthread_exit(NULL); 
  }

  
  // Etape 2 :
  
  // je demande à passer : je passe s'il y a moins de
  // maxNbJoueursEtape2 joueurs présents dans cette étape, sinon
  // j'attends que ça soit le cas.
  if(n_verrou_lock(&(param->nvTab[1]))!= 0) {
    perror("probleme n_verrou lock");
    free(param);
    pthread_exit(NULL);
  }

  pthread_mutex_lock(&vStdOut); printf("Joueur %d: debut etape 2\n", param -> indice); pthread_mutex_unlock(&vStdOut);
  
  etape();
  
  pthread_mutex_lock(&vStdOut); printf("Joueur %d: fin etape 2\n", param -> indice); pthread_mutex_unlock(&vStdOut);
  
  // je viens de finir une étape, je dois le signaler.
  if(n_verrou_unlock(&(param->nvTab[1])) != 0) {
    perror("probleme n_verrou unlock");
    free(param);
    pthread_exit(NULL); 
  }
  
  // Etape 3 :
  
  // je demande à passer : je passe s'il y a moins de
  // maxNbJoueursEtape1 joueurs présents dans cette étape, sinon
  // j'attends que ça soit le cas.
  if(n_verrou_lock(&(param->nvTab[2])) != 0) {
    perror("probleme n_verrou lock");
    free(param);
    pthread_exit(NULL);
  }
  
  pthread_mutex_lock(&vStdOut); printf("Joueur %d: debut etape 3\n", param -> indice);; pthread_mutex_unlock(&vStdOut);

  etape();
  
  pthread_mutex_lock(&vStdOut); printf("Joueur %d: fin etape 3, je termine\n", param -> indice); pthread_mutex_unlock(&vStdOut);
  
  // je viens de finir une étape, je dois le signaler.
  if(n_verrou_unlock(&(param->nvTab[2])) != 0) {
    perror("probleme n_verrou unlock");
    free(param);
    pthread_exit(NULL); 
  }

  free(param);
  pthread_exit(NULL);
}

int main(int argc, char * argv[]){
  
  if(argc != 5){
    printf ("lancement : ./bin/jeu NbJoueurs maxNbJoueursEtape1 maNbJoueursEtape2 maxNbJoueursEtape3 \n");
    exit(1);
  }
  
  srand(time(0)); // garder cette ligne sans la modifier.

  int nb_joueurs = atoi(argv[1]);
  int max_nb_joueurs_1 = atoi(argv[2]);
  int max_nb_joueurs_2 = atoi(argv[3]);
  int max_nb_joueurs_3 = atoi(argv[4]);

  if(max_nb_joueurs_1 <= 0 || max_nb_joueurs_2 <= 0 || max_nb_joueurs_3 <= 0){
    printf("attention : le nombre de joueurs doit etre superieur dans toutes les etapes\n");
    exit(2);
  }

  pthread_t* thread_ids = (pthread_t*)malloc(nb_joueurs * sizeof(pthread_t));
  params** params_joueur = (params**)malloc(nb_joueurs * sizeof(params));

  n_verrou nvTab[3];

  n_verrou_init(&nvTab[0], max_nb_joueurs_1);
  n_verrou_init(&nvTab[1], max_nb_joueurs_2);
  n_verrou_init(&nvTab[2], max_nb_joueurs_3);

  for(int i = 0; i < nb_joueurs; ++i){
  	params_joueur[i] = (params*)malloc(sizeof(params));
  	params_joueur[i]->indice = i;
  	params_joueur[i]->nvTab = nvTab;
  }

  for(int i = 0; i < nb_joueurs; ++i){

    if(pthread_create(&thread_ids[i], NULL, jouer, params_joueur[i]) != 0)
      printf("Thread error !\n");
  }

  for(int i = 0; i < nb_joueurs; ++i){
    pthread_join(thread_ids[i], NULL);
  }

  free(thread_ids);
  free(params_joueur);
  
  return 0;
}
