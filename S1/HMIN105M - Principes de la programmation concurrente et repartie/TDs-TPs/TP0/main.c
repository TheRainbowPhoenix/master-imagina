#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

const int string_size = 1024; 

void print_array(int array[], int array_size){
	for (int i = 0; i < array_size; ++i){
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

	for (int i = 0; i < array_size; i++){
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

	do {
		pid = fork();
	} while((pid == -1) && errno == EAGAIN);

	return pid;
}

void child_process(void){
	sleep(5);
}

void large_process(int largeur){

	if(largeur > 0){

		pid_t pid = -1;

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){ //fils
			printf("debut process %d\n", getpid());
			child_process();
			printf("fin process %d\n", getpid());
		}
		else{
			large_process(largeur - 1);
			/* attend tous les fils */
			while(wait(0) != -1);
		}
	}	
}

void deep_process(int hauteur){

	if(hauteur > 0){

		pid_t pid = -1;

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){ //fils
			deep_process(hauteur - 1);
		}
		else{
			printf("debut process %d\n", getpid());
			child_process();
			printf("fin process %d\n", getpid());
			/* attend tous les fils */
			while(wait(0) != -1);
		}
	}
}

void tree_process(int hauteur){
		
	if(hauteur > 0){

		pid_t pid = -1;

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){ //fils
			pid_t pid_hauteur = -1;
			pid_hauteur = create_process();

			if(pid_hauteur == -1){
				printf("error fork\n");
			}
			else if(pid_hauteur == 0){ //fils
				tree_process(hauteur - 1);
			}
			else{
				tree_process(hauteur - 1);
			}
			while(wait(0) != -1);
		}
		else{
			printf("debut process %d\n", getpid());
			child_process();
			printf("fin process %d\n", getpid());
			/* attend tous les fils */
			while(wait(0) != -1);
		}
	}
}

void process_tree(int largeur, int hauteur){
		
	if(largeur > 0){

		pid_t pid_largeur = create_process();

		if(pid_largeur == -1){
			printf("error fork\n");
		}
		else if(pid_largeur == 0){ //fils

			if(hauteur > 0){
				process_tree(largeur, hauteur - 1);
				child_process();
				/*
				pid_t pid_hauteur = create_process();

				if(pid_hauteur == -1){
					printf("error fork\n");
				}
				else if(pid_hauteur == 0){ //fils
					child_process();
				}
				else{
					process_tree(largeur, hauteur - 1);
					
					//while(wait(0) != -1);
				}
				*/
				//process_tree(largeur, hauteur - 1);

			}
		}
		else{
			process_tree(largeur, hauteur);
			/* attend tous les fils */
			while(wait(0) != -1);
		}
	}
}

void print_process_tree(void (*process_tree)(int, int), int largeur, int hauteur){

	pid_t pid = create_process();

	if(pid == -1){
		printf("error fork\n");
	}
	else if(pid == 0){ //fils
		(*process_tree)(largeur, hauteur);
	}
	else{

		char pstree_command[string_size];
		
		snprintf(pstree_command, string_size, "pstree -p %d", pid);
		
		system(pstree_command);

		/* attend tous les fils */
		while(wait(0) != -1);
	}
}

int main(/*int argc, const char *argv[]*/) {

	srand(time(NULL));

   /* Les réponses aux questions sont délimitées dans des champs 
    * pour bien les séparer et pour pouvoir redefinir des variables 
    * avec le même nom */

	printf("EXERCICE 1\n\n");

	printf("Question 2\n\n"); 
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

	printf("Question 3\n\n");
	{
		int array_size = 0;

		//scanf("%d", &array_size);
		array_size = 1 + (rand() % 30);

		int value;
		int* array = (int*)malloc(sizeof(int) * array_size);

		for (int i = 0; i < array_size; ++i){
			//scanf("%d", &value);
			value = rand() % 1001;
			array[i] = value;
		}

		printf("T = [ "); print_array(array, array_size); printf("]\n\n");
		printf("somme_rec(T) = %d\n\n", somme_rec(array, array_size));

		free(array);
	}

	printf("EXERCICE 2\n\n");

	printf("Question 1.1\n\n");
	{
		int largeur = 3;
		int hauteur = 3;

		printf("largeur = %d, hauteur = %d\n\n", largeur, hauteur);
		
		print_process_tree(&process_tree, largeur, hauteur);
	}
/*

*/
	//./run & pstree -p $!

	//deep_process(6);

	//tree_process(3);

	return 0;
}
