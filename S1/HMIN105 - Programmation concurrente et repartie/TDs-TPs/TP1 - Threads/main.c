#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

void array_init_rand(int* array, int array_size);
void array_print(int* array, int array_size);

void rand_sleep(int min, int max);
void print_tab(int n);

// EXERCICE 1

/*  1. on lance dans plusieurs threads une fonction qui attend quelques secondes
	on verifiera ensuite que tout les threads finissent en même temps.

	2. si la tache principale ce termine tout les threads se termineront

	3. si une des tâches appel exit() le processus ce termine
	(tous les threads aussi donc) */

void* sleep_thread(void* time);
void show_multithread(int thread_number, int thread_time);

// EXERCICE 2

struct prod_arg{
	int p1;
	int p2;
};

int scalar_sum = 0;
pthread_mutex_t scalar_sum_lock = PTHREAD_MUTEX_INITIALIZER;

void* product_thread(void* prod_arg);
int scalar_product_multithread(int* v1, int* v2, int v_size);

// EXERCICE 3

int is_sync;
int sync_number;
pthread_mutex_t sync_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sync_cond = PTHREAD_COND_INITIALIZER;

void* sync_thread(void*);
void sync_multithread(int thread_number, int sync_time);

// EXERCICE 4

struct process_sync_param {
	int zone_number;
	int thread_index;
};

int* process_progress;
pthread_mutex_t process_progress_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t process_progress_cond = PTHREAD_COND_INITIALIZER;

void* process_sync_thread(void* params);
void process_sync_multithread(int thread_number, int zone_number);

//////////////////////////////////

int main(int argc, char* argv[]){

	srand(time(0));

	printf("# EXERCICE 1\n\n"); {

		const int N = 4;
		const int T = 1;

		printf("Thread duration T = %d\n", T);
		printf("Thread number N = %d\n\n", N);

		printf("show_multithread(N, T):\n\n");

		show_multithread(N, T); printf("\n");
	}

	printf("# EXERCICE 2\n\n"); {

		const int N = 3;

		int v1[N];
		int v2[N];

		array_init_rand(v1, N);
		array_init_rand(v2, N);

		printf("Vector size N = %d\n\n", N);

		printf("v1 = { "); array_print(v1, N); printf("}\n");
		printf("v2 = { "); array_print(v2, N); printf("}\n\n");

		printf("scalar_product_multithread(v1, v2) = %d\n\n", scalar_product_multithread(v1, v2, N));
	}

	printf("# EXERCICE 3\n\n"); {

		const int N = 4;
		const int T = 1;

		printf("Sync duration T = %d\n", T);
		printf("Thread number N = %d\n\n", N);

		printf("sync_multithread(N, T):\n\n");

		sync_multithread(N, T); printf("\n");
	}

	printf("# EXERCICE 4\n\n"); {

		const int N = 3;
		const int Z = 5;

		printf("Thread Number N = %d\n", N);
		printf("Zone Number Z = %d\n", Z);

		printf("process_sync_multithread(N, Z):\n\n");

		process_sync_multithread(N, Z); printf("\n");
	}

	return 0;
}

//////////////////////////////////

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

void rand_sleep(int min, int max){
	sleep(min + rand() % abs(max - min));
}

void print_tab(int n){
	for (int i = 0; i < n; ++i)
		printf("\t");
}

// EXERCICE 1

void* sleep_thread(void* time){

	int T = *(int*)time;

	printf("Thread %lu will run %ds\n", pthread_self(), T);
	sleep(T);
	printf("Thread %lu ended after %ds\n", pthread_self(), T);

	pthread_exit(NULL);
}

void show_multithread(int thread_number, int thread_time){

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

int scalar_product_multithread(int* v1, int* v2, int v_size){

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

void sync_multithread(int thread_number, int sync_time){

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

	free(thread_ids);
}

// EXERCICE 4

void* process_sync_thread(void* params){

	struct process_sync_param* param = (struct process_sync_param*)params;

	int zone_number = (int)param->zone_number;
	int thread_index = (int)param->thread_index;

	for(int i = 0; i < zone_number; ++i){

		if(thread_index > 0){

			pthread_mutex_lock(&process_progress_mutex);
			while(process_progress[thread_index - 1] <= i){
				pthread_cond_wait(&process_progress_cond, &process_progress_mutex);
			}
			pthread_mutex_unlock(&process_progress_mutex);
		}

		print_tab(thread_index); printf("Thread n°%d zone n°%d is processing\n", thread_index, i);
		rand_sleep(1, 5);
		print_tab(thread_index); printf("Thread n°%d zone n°%d is finished !\n", thread_index, i);

		pthread_mutex_lock(&process_progress_mutex);
		process_progress[thread_index]++;
		pthread_cond_broadcast(&process_progress_cond);
		pthread_mutex_unlock(&process_progress_mutex);
	}

	pthread_exit(NULL);
}

void process_sync_multithread(int thread_number, int zone_number){

	process_progress = (int*)calloc(thread_number, sizeof(int));

	pthread_t* thread_ids = (pthread_t*)malloc(thread_number * sizeof(pthread_t));
	struct process_sync_param* params = (struct process_sync_param*)malloc(thread_number * sizeof(struct process_sync_param));

	for(int i = 0; i < thread_number; ++i){
		params[i] = (struct process_sync_param){zone_number, i};
		if(pthread_create(&thread_ids[i], NULL, process_sync_thread, &params[i]) != 0)
			printf("Thread error !\n");
	}

	for(int i = 0; i < thread_number; ++i){
		pthread_join(thread_ids[i], NULL);
	}

	free(process_progress);
	free(thread_ids);
	free(params);
}
