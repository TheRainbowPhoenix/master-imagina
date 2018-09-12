#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/*

Moy(0,6 ECRIT, 0.4 TP)

# EXERCICE 1

```c
int a = 10; 
int b = 25;
int* p = &b;
int* pp = &a;
```
1. la valeur de b

2. la valeur de a

3. la valeur de a

4. la valeur de b

5. la valeur de a


 */

void print_T(int T[], int T_size){
	for (int i = 0; i < T_size; ++i){
			printf("%d ", T[i]);
	}
	printf("\n");
}

int* extract(int T[], int T_size, int a, int b){
	
	int* extract_T = NULL;
	int extract_size = 0; 
	int k = 0;

	for(int i = 0 ; i < T_size ; i++){
		if(T[i] >= a && T[i] <= b)
			extract_size++;
	}

	extract_T = (int*)malloc(sizeof(int) * extract_size);

	for (int i = 0; i < T_size; i++){
		if(T[i] >= a && T[i] <= b){
			extract_T[k] = T[i];
			k++;
		}
	}

	printf("extract_size : %d\n", extract_size);
	printf("extract : ");
	print_T(extract_T, extract_size);

	return extract_T;
}

int somme_rec(int T[], int T_size){
	return T_size == 0 ? 0 : T[0] + somme_rec(T + 1, T_size - 1);
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
	
	pid_t pid = -1;

	for (int i = 0; i < hauteur; ++i) {

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid != 0){ //fils
			printf("debut process %d\n", getpid());
			child_process();
			printf("fin process %d\n", getpid());
		}
		else{
			/* attend tous les fils */
			while(wait(0) != -1);
		}
	}
}

void tree_process(int largeur, int hauteur){
	
	pid_t pid = -1;

	for (int i = 0; i < hauteur; ++i) {

		pid = create_process();

		if(pid == -1){
			printf("error fork\n");
		}
		else if(pid == 0){ //père
			large_process(largeur);
			wait(NULL);
		}
		else{
			deep_process(hauteur);
		}
	}
}

int main(/*int argc, const char *argv[]*/) {
/*
	int T[] = {1, 3, 5, 7, 9};
	int T_size = 5;

	int a = 2;
	int b = 5;

	printf("T_size : %d\n", T_size);
	printf("T : ");
	print_T(T, T_size);

	printf("\n");

	int* extract_T = extract(T, T_size, a, b);
	
	free(extract_T);
*/

/*
	int T_size;

	printf("taille? ");

	scanf("%d", &T_size);

	int* T = (int*)malloc(sizeof(int) * T_size);

	for (int i = 0; i < T_size; ++i){
		int val;
		scanf("%d", &val);
		T[i] = val;
	}

	printf("T : ");
	print_T(T, T_size);
	printf("\n");

	printf("la somme est : %d\n", somme_rec(T, T_size));
*/
	//./run & pstree -p $!
	large_process(6);

	//deep_process(6);

	//tree_process(2, 2);
	return 0;
}
