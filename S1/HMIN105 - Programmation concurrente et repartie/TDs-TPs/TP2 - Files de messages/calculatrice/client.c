#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "warn.h"
#include "calc.h"

int main(int argc, char* argv[]){

	PRINT_USAGE_IF(argc < 2, "Usage %s <FILE_ID>\n", argv[0]);

	int file_id = atoi(argv[1]);

	calc_msg calc;
	eval_msg eval;

	calc.mtype = getpid();

	while (1) {
		printf("Calcul : ");

		scanf("%d %c %d", &calc.x, &calc.symb, &calc.y);

		printf("Envoi de la requete : %d %c %d d'etiquette %ld dans la file d'id %d...\n", calc.x, calc.symb, calc.y, calc.mtype, file_id);

		int error;

		error = msgsnd(file_id, &calc, sizeof(calc), 0); WARN_ERROR(error);
		
		error = msgrcv(file_id, &eval, sizeof(eval), calc.mtype, 0); WARN_ERROR(error);

		printf("Resultat : %d\n", eval.res);
	}

	return 0;
}
