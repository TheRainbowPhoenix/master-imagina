#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <fcntl.h>

#include "warn.h"

struct sembuf op[] = {
    { 0, -1, SEM_UNDO }, //lock
    { 0, 1, SEM_UNDO } //unlock
};  

void signal_handler(int signo){

}

int main(int argc, char const *argv[]) {

    const char* filepath = "parking_shared.txt";

    printf("Creation de la clé d'acces IPC...\n");

    key_t key = ftok(filepath, 1); 
    WARN_ERROR(key);

    printf("Accès au segment de mémoire partagée...\n");

    int sh_id = shmget(key, 0, 0666);
    WARN_ERROR(sh_id);

    int sem_id = semget(key, 1, IPC_CREAT|0666);
    WARN_ERROR(sem_id);

    int error;

    while (1) {

        sleep(1);

        error = semop(sem_id, op, 1);
        WARN_ERROR(error);  

        int* nb_places = (int*)shmat(key, NULL, SHM_W);
        WARN_IF(*nb_places == -1);

        if (*nb_places > 0) {
            printf("Demande accepté\n");
            (*nb_places)--;
            printf("Impression ticket\n");
            printf("Nombre de place restante : %d", (*nb_places));
        } else {
            printf("Pas de place");
        }

        error = semop(sem_id, op+1, 1);
        WARN_ERROR(error);  

    }

    return 0;
}

/*
gestionnaire de parking

place limité

voiture demande ticket via plusieurs bornes 

distribution ticket si places libre sinon message d'erreur nb_place--

voiture quitte parking nb_place++

programme borne_arrivé -> memoire partagé RDWR -> nb_place
programme borne_sortie -> memoire partagé RDWR -> nb_place
programme setup_parking -> memoire partagé RDWR -> nb_place

*/