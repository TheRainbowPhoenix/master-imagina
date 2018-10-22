#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void array_init_rand(int* array, int array_size);
void array_print(int* array, int array_size);

// EXERCICE 1

/*  1. on lance dans plusieurs threads une fonction qui attend quelques secondes
	on verifiera ensuite que tout les threads finissent en même temps.

	2. si la tache principale ce termine tout les threads se termineront

	3. si une des tâches appel exit() le processus ce termine 
	(tous les threads aussi donc) */

void* sleep_thread(void* time);
void show_multithreading(int thread_number, int thread_time);

// EXERCICE 2

struct prod_arg{ 
	int p1;
	int p2; 
};

int scalar_sum = 0;
pthread_mutex_t scalar_sum_lock = PTHREAD_MUTEX_INITIALIZER;

void* product_thread(void* prod_arg);
int scalar_product_multithreading(int* v1, int* v2, int v_size);

// EXERCICE 3

int is_sync;
int sync_number;
pthread_mutex_t sync_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sync_cond = PTHREAD_COND_INITIALIZER;

void* sync_thread(void*);
void sync_multithreading(int thread_number, int sync_time);

// EXERCICE 4

int main(int argc, char* argv[]){

	srand(time(0));

	printf("EXERCICE 1\n\n"); {

		const int N = 4;
		const int T = 0;

		printf("Thread duration T = %d\n", T);
		printf("Thread number N = %d\n\n", N);

		printf("show_multithreading(N, T):\n\n");

		show_multithreading(N, T); printf("\n");
	}

	printf("EXERCICE 2\n\n"); {

		const int N = 3;

		int v1[N];
		int v2[N];

		array_init_rand(v1, N);
		array_init_rand(v2, N);

		printf("Vector size N = %d\n\n", N);

		printf("v1 = { "); array_print(v1, N); printf("}\n");
		printf("v2 = { "); array_print(v2, N); printf("}\n\n");

		printf("scalar_product_multithreading(v1, v2) = %d\n\n", scalar_product_multithreading(v1, v2, N));
	}

	printf("EXERCICE 3\n\n"); {

		const int N = 4;
		const int T = 0;

		printf("Sync duration T = %d\n", T);
		printf("Thread number N = %d\n\n", N);

		printf("sync_multithreading(N, T):\n\n");

		sync_multithreading(N, T); printf("\n");
	}

	printf("EXERCICE 4\n"); {

	}

	return 0;
}

void array_init_rand(int* array, int array_size){
	for (int i = 0; i < array_size ; ++i){
		array[i] = rand() % 20;
	}
}

void array_print(int* array, int array_size){
	for(int i = 0; i < array_size; ++i){
		printf("%d ", array[i]);
	}
}

// EXERCICE 1

void* sleep_thread(void* time){

	int T = *(int*)time;

	printf("Thread %lu will run %ds\n", pthread_self(), T);
	sleep(T);
	printf("Thread %lu ended after %ds\n", pthread_self(), T);
	
	pthread_exit(NULL);
}

void show_multithreading(int thread_number, int thread_time){

	pthread_t* thread_ids = (pthread_t*)malloc(thread_number * sizeof(pthread_t));

	for(int i = 0; i < thread_number; ++i){
		if(pthread_create(&thread_ids[i], NULL, sleep_thread, &thread_time) != 0)
			printf("Thread error !\n");
	}

	for(int i = 0; i < thread_number; ++i){
		pthread_join(thread_ids[i], NULL);
	}

	free(thread_ids);
}

// EXERCICE 2

void* product_thread(void* prod_arg){

	struct prod_arg* arg = (struct prod_arg*)prod_arg;

	pthread_mutex_lock(&scalar_sum_lock);
	scalar_sum += arg->p1 * arg->p2;
	pthread_mutex_unlock(&scalar_sum_lock);

	pthread_exit(NULL);
}

int scalar_product_multithreading(int* v1, int* v2, int v_size){

	scalar_sum = 0;

	pthread_t* thread_ids = (pthread_t*)malloc(v_size * sizeof(pthread_t));
	struct prod_arg* prod_args = (struct prod_arg*)malloc(v_size * sizeof(struct prod_arg));

	for(int i = 0; i < v_size; ++i){
		prod_args[i] = (struct prod_arg){v1[i], v2[i]};
		if(pthread_create(&thread_ids[i], NULL, product_thread, &prod_args[i]) != 0)
			printf("Thread error !\n");
	}

	for(int i = 0; i < v_size; ++i){
		pthread_join(thread_ids[i], NULL);
	}

	pthread_mutex_destroy(&scalar_sum_lock);
	free(thread_ids);
	free(prod_args);

	return scalar_sum;
}

// EXERCICE 3

void* sync_thread(void* thread_number){

	printf("Thread %lu is synchronising...\n", pthread_self());

	pthread_mutex_lock(&sync_lock);

	while(!is_sync){
		pthread_cond_wait(&sync_cond, &sync_lock);
	}
	
	pthread_mutex_unlock(&sync_lock);

	printf("Thread %lu synchronised !\n", pthread_self());

	pthread_exit(NULL);
}

void sync_multithreading(int thread_number, int sync_time){
	
	is_sync = 0;
	sync_number = 0;
	pthread_t* thread_ids = (pthread_t*)malloc(thread_number * sizeof(pthread_t));

	for(int i = 0; i < thread_number; i++){
		if(pthread_create(&thread_ids[i], NULL, sync_thread, &thread_number) != 0)
			printf("Thread error !\n");
	}
	
	sleep(sync_time);

	pthread_mutex_lock(&sync_lock);
	
	is_sync = 1;
	
	printf("Woking up every threads...\n");
	
	pthread_cond_broadcast(&sync_cond);
	
	pthread_mutex_unlock(&sync_lock);

	for(int i = 0; i < thread_number; ++i){
		pthread_join(thread_ids[i], NULL);
	}

	pthread_cond_destroy(&sync_cond);
	free(thread_ids);
}

// EXERCICE 4


