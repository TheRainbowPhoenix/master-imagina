#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/*

# EXERCICE 1

1. on lance dans plusieurs threads un fonction qui attend 5 secondes
on verifiera ensuite que tout les threads finissent en même temps.

2. si la tache principale ce termine tout les threads se termineront

3. si une des tâches appel exit() le processus ce termine 
(tous les threads aussi donc)

4. 

# EXERCICE 2

1. 

*/

void* sleeper(void* arg){
	pthread_t thread_id = pthread_self();

	printf("start thread %lu pid %d\n", thread_id, getpid());

	sleep(5);

	printf("end thread %lu pid %d\n", thread_id, getpid());
	
	pthread_exit(NULL);
}

#define VECT_SIZE 10

struct vector {
	int* data;
	int size;
};

struct param {
	struct vector* vect;
	long long produit;
};

void vector_create(struct vector* v, int size){
	v->data = (int*)malloc(size * sizeof(int)); 
	v->size = size;
}

void vector_init_rand(struct vector* v){
	for (int i = 0; i < v->size ; ++i){
		v->data[i] = rand() % 100;
	}
}

void vector_print(struct vector* v){
	
	printf("[ ");
	for (int i = 0; i < v->size ; ++i){
		printf("%d ", v->data[i]);
	}
	printf("]\n");
}

void vector_free(struct vector* v, int size){
	free(v->data); 
	v->size = 0;
}

long long vector_produit(struct vector* v){
	
	long long prods = 1;
	
	for (int i = 0; i < v->size; ++i){
		prods *= (long long)v->data[i];	
	}

	return prods;
}

void* produit(void* param){
	struct param* p = (struct param*)param;
	
	p->produit = vector_produit(p->vect);
	
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	srand(time(0));
	/*
	pthread_t thread_ids[5];

	printf("debut du main\n");

	for(int i = 0 ; i < 5 ; i++){

		if(pthread_create(&thread_ids[i], NULL, sleeper, NULL) != 0){
			printf("thread error\n");
		}
	}
	
	for(int i = 0 ; i < 5 ; i++){
		pthread_join(thread_ids[i], NULL);
	}

	printf("fin du main\n");
*/

	const int N = 3;
	int size = 3;
	
	struct vector vectors[N];
	struct param params[N];

	for (int i = 0; i < N; ++i){
		vector_create(&vectors[i], size);
		vector_init_rand(&vectors[i]);
		vector_print(&vectors[i]);

		params[i].vect = &vectors[i];
		params[i].produit = 0;
	}

	pthread_t thread_ids[N];

	for (int i = 0; i < N; ++i){
		if(pthread_create(&thread_ids[i], NULL, produit, &params[i]) != 0){
			printf("thread error\n");
		}
	}

	for(int i = 0 ; i < N ; i++){
		pthread_join(thread_ids[i], NULL);
	}

	long long sum = 0;

	for(int i = 0 ; i < N ; i++){
		sum += params[i].produit; 
	}

	printf("total sum : %lld\n", sum);

	return 0;
}