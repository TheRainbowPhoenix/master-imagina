#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <fcntl.h>

#include "warn.h"
#include "calc.h"

// Evalue le calcule et reponde a l'envoyeur:
int calc_plus(calc_msg* msg, int queue_id){
    eval_msg eval;
    eval.mtype = msg->mtype;
    eval.res = msg->x + msg->y;

    int error = msgsnd(queue_id, &eval, sizeof(eval), 0); WARN_ERROR(error);

    return eval.res;
}

int calc_minus(calc_msg* msg, int queue_id){
    eval_msg eval;
    eval.mtype = msg->mtype;
    eval.res = msg->x - msg->y;

    int error = msgsnd(queue_id, &eval, sizeof(eval), 0); WARN_ERROR(error);

    return eval.res;
}

int calc_mult(calc_msg* msg, int queue_id){
    eval_msg eval;
    eval.mtype = msg->mtype;
    eval.res = msg->x * msg->y;

    int error = msgsnd(queue_id, &eval, sizeof(eval), 0); WARN_ERROR(error);

    return eval.res;
}

int calc_div(calc_msg* msg, int queue_id){
    eval_msg eval;
    eval.mtype = msg->mtype;
    eval.res = msg->y != 0 ? msg->x + msg->y : -1;

    int error = msgsnd(queue_id, &eval, sizeof(eval), 0); WARN_ERROR(error);

    return eval.res;
}

int eval_calc_msg(calc_msg* msg, int queue_id){
    switch(msg->symb){
        case '+':
            return calc_plus(msg, queue_id);
            break;
        case '-':
            return calc_minus(msg, queue_id);
            break;
        case '*':
            return calc_mult(msg, queue_id);
            break;
        case '/':
            return calc_div(msg, queue_id);
            break;
        default:
            printf("Type de calcul inconnue !\n");
            return -1;
            break;
    }
}

int main() {
    printf("Creation de la file de message...\n");

    int queue_id = msgget(IPC_PRIVATE, O_RDONLY); WARN_ERROR(queue_id);

    printf("ID FILE : %d\n", queue_id);

    printf("Creation des calculatrices...\n");
    
    int error, res;
    calc_msg calc;
    
    while (1) {
        error = msgrcv(queue_id, &calc, sizeof(calc), 0, 0); WARN_ERROR(error);

        res = eval_calc_msg(&calc, queue_id);

        printf("Requete + reçu : %d %c %d\n", calc.x, (char)calc.symb, calc.y);
        printf("Resultat : %d\n", res);
    }

    printf("Destruction de la file de message...\n");
    
    error = msgctl(queue_id, IPC_RMID, NULL); WARN_ERROR(error);

	return 0;
}