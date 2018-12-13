#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <fcntl.h>

#include "warn.h"

#define DECOMPOSEUR 1
#define PARENT 2

typedef struct num_msg {
    long mtype;
    long long num;
} num_msg;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prime_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t factor_cond = PTHREAD_COND_INITIALIZER;

int factors_to_eat_num = 0;
int primes_to_eat_num = 0;

// Si le nombre n'est pas premiers stock les facteurs dans a et b sinon et
// renvoie vrai, sinon renvoie faux et ne stock rien dans a et b.
long long factors(long long num, long long* a, long long* b){
    for (long long i = 2; i < sqrt(num); ++i) {
        if (num % i == 0) {
            *a = i;
            *b = num / i;
            return 1;
        }
    }
    return 0;
}

void* decomposeur(void* queue_id) {

    int error;
    int qid = *(int*)queue_id;
    num_msg dcmp = {DECOMPOSEUR, 0};
    num_msg dcmp_f1 = {DECOMPOSEUR, 0};
    num_msg dcmp_f2 = {DECOMPOSEUR, 0};
    num_msg dcmp_prime = {PARENT, 0};
    
    printf("Decomposeur %lu : started\n", pthread_self());

    while (dcmp.num != -1) {

        pthread_mutex_lock(&queue_mutex);
        
        while (factors_to_eat_num <= 0) {
            printf("Decomposeur %lu : waiting for factors...\n", pthread_self());
            pthread_cond_wait(&factor_cond, &queue_mutex);
        }
        
        printf("Decomposeur %lu : msgrcv factor...\n", pthread_self());
        error = msgrcv(qid, &dcmp, sizeof(dcmp.num), DECOMPOSEUR, 0); WARN_ERROR(error);
        factors_to_eat_num--;

        pthread_mutex_unlock(&queue_mutex);

        if (dcmp.num != -1) {

            printf("Decomposeur %lu : evaluating factors(%lld)...\n", pthread_self(), dcmp.num);

            if (factors(dcmp.num ,&dcmp_f1.num, &dcmp_f2.num)) {
                printf("Decomposeur %lu : factors(%lld) = (%lld, %lld)\n", pthread_self(), dcmp.num, dcmp_f1.num, dcmp_f2.num);
                pthread_mutex_lock(&queue_mutex);
                error = msgsnd(qid, &dcmp_f1, sizeof(dcmp_f1.num), 0); WARN_ERROR(error);
                error = msgsnd(qid, &dcmp_f2, sizeof(dcmp_f2.num), 0); WARN_ERROR(error);
                factors_to_eat_num += 2;
                pthread_cond_broadcast(&factor_cond);
                pthread_mutex_unlock(&queue_mutex);
            } else {
                dcmp_prime.num = dcmp.num;
                printf("Decomposeur %lu : %lld is prime\n", pthread_self(), dcmp_prime.num);
                pthread_mutex_lock(&queue_mutex);
                error = msgsnd(qid, &dcmp_prime, sizeof(dcmp_prime.num), 0); WARN_ERROR(error);
                primes_to_eat_num++;
                pthread_cond_signal(&prime_cond);
                pthread_mutex_unlock(&queue_mutex);
            }
        }
    }

    printf("Decomposeur %lu : stopped\n", pthread_self());

    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

    PRINT_USAGE_IF(argc < 3, "Usage %s <NUM> <THREAD_NUM>", argv[0]);

    int error;

    num_msg dcmp_init = {DECOMPOSEUR, atoll(argv[1])};
    int thread_num = atoi(argv[2]);

    printf("Creation de la file de message...\n");

    int queue_id = msgget(IPC_PRIVATE, 0666); WARN_ERROR(queue_id);

    printf("ID FILE : %d\n", queue_id);

    printf("Decomposition de %lld:\n", dcmp_init.num);

    error = msgsnd(queue_id, &dcmp_init, sizeof(dcmp_init.num), 0); WARN_ERROR(error);
    factors_to_eat_num++;

    printf("Creation de %d décomposeurs...\n", thread_num);

    pthread_t* thread_ids = (pthread_t*)malloc(thread_num * sizeof(pthread_t));

    for (int i = 0; i < thread_num; ++i) {
        error = pthread_create(&thread_ids[i], NULL, &decomposeur, &queue_id); WARN_ERROR_PTHREAD(error);
    }

    long long current_num = 1;
    num_msg dcmp_current;

    pthread_mutex_lock(&queue_mutex);

    while (current_num != dcmp_init.num) {
        
        printf("Parent : waiting for primes...\n");
        pthread_cond_wait(&prime_cond, &queue_mutex);
        
        while (primes_to_eat_num > 0) {
            printf("Parent : msgrcv primes...\n");
            error = msgrcv(queue_id, &dcmp_current, sizeof(dcmp_current.num), PARENT, 0); WARN_ERROR(error);
            current_num *= dcmp_current.num;
            printf("%lld *\n", dcmp_current.num);
            primes_to_eat_num--;
        }
    }

    /* Message pour terminer les decomposeurs */
    num_msg dcmp_end = {DECOMPOSEUR, -1};
    
    printf("Envoie du signal de fin aux decomposeurs...\n");

    for (int i = 0; i < thread_num; ++i) {
        error = msgsnd(queue_id, &dcmp_end, sizeof(dcmp_end.num), 0); WARN_ERROR(error);
        factors_to_eat_num++;
    }

    pthread_cond_broadcast(&factor_cond);
    pthread_mutex_unlock(&queue_mutex);

    printf("Attente des decomposeur...\n");
    
    for (int i = 0; i < thread_num; ++i) {
        error = pthread_join(thread_ids[i], NULL); WARN_ERROR_PTHREAD(error);
    }

    printf("Destruction de la file de message...\n");
    
    error = msgctl(queue_id, IPC_RMID, NULL); WARN_ERROR(error);

    return 0;
}



/*

struct shared_segment {
    int socket_fds_size = MAX_CLIENT;
    int client_current_num = 0;
    int socket_fds[MAX_CLIENT] = {-1};  
};

#define MEM_SEG_KEY 12345

const char* msg_data_file_path = "msg_data.txt";

connection_request(server_sock);

pid_t fork_process(void){
    
    pid_t pid;

    do{
        pid = fork();
    } 
    while((pid == -1) && errno == EAGAIN);

    return pid;
}

int main(int argc, char const *argv[]) {
    
    PRINT_USAGE_IF(argc < 2, "Usage %s <PORT>", argv[0]);
    
    create_segment(){
    
    }

    create_socket()

    wait_client() {
        
        while(true){
            client_socket = accept(connection_sock))

            fork
            si fils 
                manage_client(client_socket);
            sinon
                continue
        }

    }

    manage_client(client_socket) {
    
        shared_data* = shmat();
            
        lock_data()
        client_id = get_client_id(shared_data);
        unlock_data();


        msg_data_size = strlen(msg_data);
        tcpsend()

        lock();
        remove_client_id(shared_data, client_id);
        unlock
    }

}


COMMUNICATIONS TCP/IP

# SERVEUR CONCURRENT:
- Gère l'accès des clients sur l'espace partagé.
- Informe les clients des modifications.

manage_client_update:
- envoie données partagées au client
- thread attente de modif, mise à jour espace partagée, diffusion nouvelle etat
- thread attente modif autre client (semaphore), diffusion etat

# CLIENT:
- Echange avec le serveur pour visualisé/modifé le contenu de l'espace partagé.

manage_server_update:
- reception données partagées du serveur
- thread modification de l'espace partagé
- thread attente mise à jour

date:username:msg


autre:
- structure partagé

server.c:

// renvoie l'id du segment
int create_segment(...)

int wait_client(...):
    wait_connection en tcp
    int manage_client(...)
    int check_update(...)
    int update_all(...)

client.c:

init_client():
    demande de connexion tcp 1 <- taille username
    demande de connexton tcp 2 <- username

    attente reponse de connexion tcp 1 -> taille data
    attente reponse de connexion tcp 2 -> data 
    
    creation fichier data;

print_chat(filepath):
    affiche le chat

threads:

void* rcv_chat_update(void*):
    attente reponse de connexion tcp 1 -> taille data
    attente reponse de connexion tcp 2 -> data 

    réécriture du fichier data

    print_chat(filepath)

void* snd_chat_msg(void*)
    getline()
    envoie message tcp 1 <- taille message
    envoie message tcp 2 <- message
*/