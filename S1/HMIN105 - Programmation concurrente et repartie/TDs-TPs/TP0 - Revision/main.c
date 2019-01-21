#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

const int string_size = 1024; 

void print_array(int array[], int array_size){

	for(int i = 0; i < array_size; ++i){
			printf("%d ", array[i]);
	}
}

int* extract(int array[], int array_size, int a, int b, int* extract_size){

	int* extract_array = NULL;

	*extract_size = 0; 

	for(int i = 0 ; i < array_size ; i++){
		if(array[i] >= a && array[i] <= b) 
			(*extract_size)++;
	}

	extract_array = (int*)malloc(sizeof(int) * (*extract_size));

	int k = 0;

	for(int i = 0; i < array_size; i++){
		if(array[i] >= a && array[i] <= b){
			extract_array[k] = array[i];
			k++;
		}
	}

	return extract_array;
}

int somme_rec(int array[], int array_size){
	return array_size == 0 ? 0 : array[0] + somme_rec(array + 1, array_size - 1);
}

pid_t create_process(void){
	
	pid_t pid;

	do{
		pid = fork();
	} 
	while((pid == -1) && errno == EAGAIN);

	return pid;
}

void child_process(void){
	sleep(2);
}

void process_large(int largeur){

	if(largeur > 0){

		pid_t pid = -1;

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){
			// printf("debut process %d\n", getpid());
			child_process();
			// printf("fin process %d\n", getpid());
		}
		else{
			process_large(largeur - 1);

			// Attend tous les fils
			while(wait(0) != -1);
		}
	}	
}

void process_deep(int hauteur){

	if(hauteur > 0){

		pid_t pid = -1;

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){
			process_deep(hauteur - 1);
		}
		else{
			// printf("debut process %d\n", getpid());
			child_process();
			// printf("fin process %d\n", getpid());
			while(wait(0) != -1);
		}
	}
}

// Lance n fork du meme processus en parrallel
void fork_ntime(void (*process)(int, int), int arg1, int arg2, int n){

	if(n > 0){

		pid_t pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){ //fils
			(*process)(arg1, arg2);
		}
		else{
			fork_ntime(process, arg1, arg2, n - 1);
		}
	}	
}

// Fork un processus sous forme d'arboressence
void process_tree(int largeur, int hauteur){

	if(hauteur > 0){

		pid_t pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){ //fils

			process_tree(largeur, hauteur - 1);
			child_process();
		}
		else{
			fork_ntime(process_tree, largeur, hauteur - 1, largeur - 1);
			child_process();
		}
	}
}

// Affiche l'arboressence du proccessus forké
void print_process_tree(void (*process_tree)(int, int), int largeur, int hauteur){

	pid_t pid = create_process();

	if(pid == -1){
		printf("error fork\n");
	}
	else if(pid == 0){
		(*process_tree)(largeur, hauteur);
	}
	else{
		char pstree_command[string_size];
		
		snprintf(pstree_command, string_size, "pstree -p %d", pid);
		
		// Attend pour permettre au process tree de ce generer
		sleep(1);
		
		system(pstree_command);

		while(wait(0) != -1);
	}
}

int main(/*int argc, const char *argv[]*/) {

	srand(time(NULL));

   /* Les réponses aux questions sont délimitées dans des champs 
    * pour bien les séparer et pour pouvoir redefinir des variables 
    * avec le même nom */

	printf("EXERCICE 1\n\n");

	printf("Question 1)\n\n"); 
	{
		printf("int a = 10;\n");
		printf("int b = 25;\n");
		printf("int* p = &b;\n");
		printf("int* pp = &a;\n\n");

		printf("1. *(&(*(*(&p))))     -> la valeur de b\n");
		printf("2. *(p-1)             -> la valeur de a\n");
		printf("3. *(*(&p)-1)         -> la valeur de a\n");
		printf("4. *(*(&pp)+1)        -> la valeur de b\n");
		printf("5. *(&(*(*(&p))) - 1) -> la valeur de a\n\n");
	}

	printf("Question 2)\n\n"); 
	{
		int array[] = {1, 3, 5, 7, 9};
		int array_size = 5;

		int a = 2;
		int b = 5;

		printf("a = %d, b = %d\n", a, b);
		printf("T = [ "); print_array(array, array_size); printf("]\n\n");

		int extract_size = 0;
		int* extract_array = extract(array, array_size, a, b, &extract_size);
		
		printf("extract(T, a, b) = [ "); print_array(extract_array, extract_size); printf("]\n\n");
		
		free(extract_array);
	}

	printf("Question 3)\n\n");
	{
		int array_size = 0;

		//scanf("%d", &array_size);
		array_size = 1 + (rand() % 30);

		int value;
		int* array = (int*)malloc(sizeof(int) * array_size);

		for(int i = 0; i < array_size; ++i){
			//scanf("%d", &value);
			value = rand() % 1001;
			array[i] = value;
		}

		printf("T = [ "); print_array(array, array_size); printf("]\n\n");
		printf("somme_rec(T) = %d\n\n", somme_rec(array, array_size));

		free(array);
	}

	printf("EXERCICE 2\n\n");
	{
		int largeur = 3;
		int hauteur = 2;

		printf("largeur = %d, hauteur = %d\n\n", largeur, hauteur);
		
		print_process_tree(&process_tree, largeur, hauteur);
	}

	// ./run & pstree -p $!
	return 0;
}
