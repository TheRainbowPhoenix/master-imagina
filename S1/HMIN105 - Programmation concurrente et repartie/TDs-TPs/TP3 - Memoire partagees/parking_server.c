#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <pthread.h>
#include <fcntl.h>

#include "warn.h"

// creation du segment de mémoire partagée.
int parking_init(const char* filepath, int places){

    printf("[PARKING_INIT] : STARTED\n");

    printf("Creation du fichier partagée...\n");

    int fd = open(filepath, O_RDWR|O_CREAT, 0666);
    WARN_ERROR(fd);

    printf("Creation de la clé d'acces IPC...\n");

    key_t key = ftok(filepath, 1); 
    WARN_ERROR(key);
    
    printf("Creation du segment de mémoire partagée...\n");

    int sh_id = shmget(key, sizeof(int), IPC_CREAT|0666);
    WARN_ERROR(sh_id);

    printf("Initialisation de la variable partagée...\n");

    int* nb_places = shmat(key, NULL, SHM_RDONLY);
    WARN_ERROR_IF((void*)nb_places == (void*)-1);

    //*nb_places = places;

    printf("Detachement du segment de mémoire partagée...\n");

    int error = shmdt((void*)nb_places);
    WARN_ERROR(error);

    printf("[PARKING_INIT] : ENDED\n");

    close(fd);

    return sh_id;
}

// destruction du segment de mémoire partagée.
int parking_destroy(int sh_id){
    return shmctl(sh_id, IPC_RMID, NULL);
}

int main(int argc, char const *argv[]) {

    PRINT_USAGE_IF(argc < 2, "Usage %s <NB_PLACES>", argv[0]);

    parking_init("parking_shared.txt", atoi(argv[1]));

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