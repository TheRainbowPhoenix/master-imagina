# HMIN105M - Programmation concurrente et repartie

## Sommaire [↺](#sommaire-)

- [Informations](#informations-)
    - [Examens](#examens-)
    - [Ressources](#ressources-)
- [Programmation concurrente](#programmation-concurrente-)
    - [Processus leger](#processus-leger-)
        - [Thread](#thread-)
        - [Mutex](#mutex-)
        - [Variable conditionnelle](#variable-conditionnelle-)
    - [Processus lourd](#precessus-lourd-)
        - [Fork](#fork-)
        - [IPCs](#ipcs-)
            - [File de message](#file-de-message-)
            - [Mémoire partagée](#memoire-partagee-)
            - [Sémaphores](#semaphores-)
- [Programmation repartie](#programmation-repartie-)
    - [Communication distante](#communication-distante-)
        - [Socket](#socket-)
        - [Protocole](#protocole-)
    - [Remote controle procedure](#remote-controle-procedure-)
        - [RPCL](#rpcl-)
        - [MPI](#mpi-)
        - [OpenMP](#openmp-)

### Examens [↺](#sommaire-)

- Note final : 60% Exam + 40% TP
- TP noté multithreading : 23/10/2018
- Projet client-serveur + IPC + multithreading : 04/12/2018

### Ressources [↺](#sommaire-)

- [Cours et TPs](https://moodle.umontpellier.fr/course/view.php?id=675)
- [System Programming Wiki](https://github.com/angrave/SystemProgramming/wiki)

## Programmation concurrente [↺](#sommaire-)

Fork                                            | Thread
----------------------------------------------- | --------------------------------------------------------------------
Processus lourd                                 | Processus leger
Changement de contexte couteux                  | Partie d'un processus
Espace d'adressage de processus non partageable | Peu partager des données en memoire avec d'autres threads
Outils de synchronisation difficiles            | Permet d'executer plusieurs unités d'execution de manière asynchrone
Plus difficile à implementer                    | Execute une fonction

### Processus leger [↺](#sommaire-)

Les processus legers sont appelés des threads, les objets et leurs operations en rapport avec le threads posix sont definis dans le fichier **pthread.h** sous la forme :

- **pthread_objet_t**
- **pthread_objet_opération()**

L'option **-lpthread** ou **-pthread** sont necessaire à la compilation.

#### Thread [↺](#sommaire-)

Sur un thread on peut effectuer les actions suivantes :

Action         | Fonction
-------------- | -----------------------------------------------------
Creation       | `int pthread_create(*thread, *attr, *function, *arg)`
Identification | `pthread_t pthread_self()`
Egalité        | `int pthread_equal(thread1, thread2)`
Attente        | `int pthread_join(thread, **retval)`
Destruction    | `void pthread_exit(*retval)`

```c
// Créée un thread en lui associant des attributs, une fonction
// et ses arguments.
int pthread_create(             // Renvoie 0 à la reusite sinon un code d'erreur != 0
    pthread_t* thread,          // Identité du thread obtenue en résullat
    const pthread_attr_t* attr, // Attribut du thread, NULL par default
    void* (*function)(void*),   // Fonction à demarrer dans le thread
    void* arg                   // Argument(s) passé à la fonction
);

// Renvoie l'identifiant du thread appelant la fonction.
pthread_t pthread_self(void);

// Renvoie un nombre != 0 si les threads sont egaux et 0 sinon.
int pthread_equal(  
    pthread_t thread1,
    pthread_t thread2
);

// Attend la fin du thread passé en paramètre et permet d'accéder
// aux données qu'il a renvoyées.
int pthread_join(     // Renvoie 0 à la reussite sinon un code d'erreur != 0
    pthread_t thread, // Identifiant du thread à attendre
    void** retval     // Donnée(s) renvoyée(s) par le thread attendu
);

// Termine le thread appelant la fonction et permet de renvoyer des
// données si le thread est joignable.
void pthread_exit(
    void* retval   // Donnée(s) renvoyée(s)
);
```

**Attention !** La fonction exit() termine le processus même si l'appelant est un thread secondaire.

#### Mutex [↺](#sommaire-)

Sur un mutex on peut effectuer les actions suivantes :

Action                    | Fonction
------------------------- | ---------------------------------------
Creation                  | `int pthread_mutex_init(*mutex, *attr)`
Verrouillage              | `int pthread_mutex_lock(*mutex)`
Verrouillage non bloquant | `int pthread_mutex_trylock(*mutex)`
Deverouillage             | `int pthread_mutex_unlock(*mutex)`
Destruction               | `int pthread_mutex_destroy(*mutex)`

**Remarque** : Toutes ces fonctions retournent 0 en cas de succès et sinon un code d'erreur ≠ 0.

```c
pthread_mutex_t mutex = PHTREAD_MUTEX_INITIALIZER; // Initialisation plus simple d'un mutex

// Créée un mutex en lui associant des attributs.
int pthread_mutex_init(
    pthread_mutex_t* mutex,   // Mutex initialisé obtenu en résullat
    pthread_mutexattr_t* attr // Attribut du mutex, NULL par default
);

// Verrouille le mutex sauf si celui-ci et déja verrouillé par un
// autre thread, dans ce cas la fonction bloquera le thread appelant
// jusqu'a ce que le mutex soit deverrouillé.
int pthread_mutex_lock(
    pthread_mutex_t* mutex
);

// Equivalent à pthread_mutex_lock() sauf que si le mutex est deja
// verrouillé, la fonction ne bloquera pas le thread appelant.
int pthread_mutex_trylock(
    pthread_mutex_t* mutex
);

// Deverouille le mutex.
int pthread_mutex_unlock(
    pthread_mutex_t* mutex
);

// Detruit le mutex.
int pthread_mutex_destroy(
    pthread_mutex_t* mutex
);
```

**Règle** : Tout accès à une variable accessible en lecture par un thread et en écriture par un autre doit être protégé.

- Un seul mutex peut protéger plusieurs variables, mais pas l'inverse.
- Les opérations lock() et unlock() sont atomiques, mais pas la portion de code qui se trouve entre les deux.
- Cette portion de code est appelée **section critique**.
- Si un thread est dans une **section critique**, il doit être garanti qu'aucun autre thread n'y sois pas simultanément c'est **l'exclusion mutuelle**.

#### Variable conditionnelle [↺](#sommaire-)

Sur une variable conditionnelle et un mutex on peut effectuer les actions suivantes :

Action                            | Fonction
--------------------------------- | ----------------------------------------------
Creation                          | `pthread_cond_init(*cond, *attr)`
Attente                           | `pthread_cond_wait(*cond, *mutex)`
Attente avec delai                | `pthread_cond_timedwait(*cond, *mutex, *time)`
Liberation d'une attente          | `pthread_cond_signal(*cond)`
Liberation de toutes les attentes | `pthread_cond_broadcast(*cond)`
Destruction                       | `pthread_cond_destroy(*cond)`

**Remarque** : Toutes ces fonctions retournent 0 en cas de succès et sinon un code d'erreur ≠ 0.

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Initialisation plus simple d'une variable conditionnelle

// Créée une variable conditionnelle en lui associant des attributs.
int pthread_cond_init(
    pthread_cond_t* cond,    // Variable conditionnelle initialisée obtenue en résullat
    pthread_condattr_t* attr // Attribut de la variable conditionnelle, NULL par default
);

// Deverrouille le mutex et bloque le thread appelant la fonction
// sur la variable conditionnelle.
// La fonction ce debloquera quand un autre thread libèrera la
// variable conditionnelle.
// Quand la fonction ce termine avec succès, le mutex est
// automatiquement verrouillé par le thread appelant.
int pthread_cond_wait(
    pthread_cond_t* cond,   
    pthread_mutex_t* mutex
);

// Equivalent à pthread_cond_wait() sauf que si le delai indiqué en
// paramètre est dépassé la fonction ce débloquera et renverra un
// code d'erreur.
int pthread_cond_timedwait(
    pthread_cond_t *cond,       
    pthread_mutex_t *mutex,     
    const struct timespec *time
);

// Libère un des threads bloqués sur la variable conditionnelle.
int pthread_cond_signal(
    pthread_cond_t* cond
);

// Libère tous les threads bloqués sur la variable conditionnelle.
int pthread_cond_broadcast(
    pthread_cond_t* cond    
);

// Detruit la variable conditionnelle.
int pthread_cond_destroy(
    pthread_cond_t* cond  
);
```

**Important :**

- La primitive suppose que le thread appelant a obtenu précédemment le verrouillage de verrou.
- Plusieurs threads peuvent être débloqués. Il est donc utile de tester à nouveau le prédicat après réveil
- Lors du réveil, les threads réveillés vont obtenir tour à tour automatiquement le verrouillage du verrou.

**Choix signal ou broadcast :**

- Utiliser signal seulement si on est certain que n'importe quelle tâche en attente peut faire le travail requis et qu'il est indispensable de réveiller une seule tâche.
- Lorsqu'une variable conditionnelle est utilisée pour plusieurs prédicats, signal est à prohiber.
- En cas de doute, utiliser broadcast.

**Déverrouiller après ou avant l'annonce :**

- Après engendre qu'un thread réveillé ne pourra pas obtenir le verrouillage immédiatement car le verrou est toujours indisponible. Donc le thread réveillé devra se bloquer temporairement.
- Avant peut être plus efficace, mais il se peut aussi qu'un thread Tz non (encore) en attente obtienne le verrouillage. Il n'y a pas d'équité, alors que le thread réveillé Ta peut être plus prioritaire (Tz moins prioritaire a obtenu le verrouillage alors que Ta, en attente de l'annonce, ne pouvait l'obtenir).

### Processus lourd [↺](#sommaire-)

#### Fork [↺](#sommaire-)

#### IPCs [↺](#sommaire-)

visualisation des information avec la commande `ipcs`.

objet ipc publique: il faut fournir une clé pour que les autres processus puissent la trouvé.

objet ipc privé: utilise la clé IPC_PRIVATE les autres processus doivent obtenir l'id de l'objet

genere un clé publique en fonction du fichier existant et de l'id du projet (>0).

```c
key_t key = ftok(const char* pahtname, int proj_id);
```

##### File de message [↺](#sommaire-)

analogie boite contenant des message avec des etiquettes

on peut extraire les message qui nous interresse uniquement

durée de vie tant que la mémoire système n'a pas été nettoyé (redemarage) objets persistants

depos de message atomique

si file pleine ecrivain endormie

si la file est vide lecteur endormie

Action                            | Fonction
--------------------------------- | ----------------------------------------------
Creation                          | `msgget(key, rights)`
Identification                    | `pthread_cond_wait(*cond, *mutex)`
Ecriture                          | `pthread_cond_timedwait(*cond, *mutex, *time)`
Lecture                           | `pthread_cond_signal(*cond)`
Controle                          | `pthread_cond_destroy(*cond)`

```c
// Créée une variable conditionnelle en lui associant des attributs.
int msgget(key_t key, int msgflg);


struct msgbuf {
    long mtype;    // Etiquette du message (doit etre > 0)
    char mtext[1]; // Données du messages (pas de pointeurs)
};

// msgtyp > 0 : lis le premier message dont l'etiquette e = msgtyp
// msgtyp = 0 : lecture du premier message disponible
// msgtyp < 0 : lecture du premier message disponible avec la plus petite etiquette e <= |msgtyp|
ssize_t msgrcv(int msgid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

// controle operation on the queue
int msgctl(int msgid, int cmd, struct msgid_ds *buf);
int msgctl(qid, IPC_RMID, NULL); // remove queue


// Renovoie une erreur si le type du message contient une valeur négative ou nulle
int msgsnd(int msgid, const void *msgp, size_t msgsz, int msgflg);

```

##### Mémoire partagée [↺](#sommaire-)

permet a plusieurs programmes d'avoir un espace commun de mémoire

Action                            | Fonction
--------------------------------- | ----------------------------------------------
Creation                          | `msgget(key, rights)`
Attachement                       | `pthread_cond_wait(*cond, *mutex)`
Dettachement                      | `pthread_cond_timedwait(*cond, *mutex, *time)`
Controle                          | `pthread_cond_signal(*cond)`

```c
int shmget(key_t key, size_t size, int shmflg);

// attachement
void *shmat(int shmid, const void *shmaddr, int shmflg); verrify les erreur avec (void*)-1

// detachement
int shmdt(const void *shmaddr);

// destruction
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

##### Sémaphores [↺](#sommaire-)

mecanisme de synchronisation de processus.

Action                            | Fonction
--------------------------------- | ----------------------------------------------
Creation                          | `msgget(key, rights)`
Attachement                       | `pthread_cond_wait(*cond, *mutex)`
Dettachement                      | `pthread_cond_timedwait(*cond, *mutex, *time)`
Controle                          | `pthread_cond_signal(*cond)`

```c
int semget(key_t key, int nsems, int semflg);

int semop(int semid,
    struct sembuf *sops, // Ensemble d'operation à réaliser
    size_t nsops); // Nombre d'operations

struct sembuf {
    unsigned short sem_num; // Numéro de la semaphore
    short sem_op; // Opération sur la semaphore
    short sem_flg; // Options (Ex SEM_UNDO)
};


int semctl(int semid, int semnum, int cmd, ...);

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

// destruction
int semctl(semid, 0, IPC_RMID);

```
- différences entre processus et thread
- partage des ressources inter-processus et inter-threads
- synchronisation entre processus et threads (exclusion mutuelle attente d'un evenement)
- Attention à la synchronisation, problème d'interblocage

## Programmation repartie [↺](#sommaire-)

### Communication distante [↺](#sommaire-)

#### Socket [↺](#sommaire-)

##### Protocole [↺](#sommaire-)

**client** : processus qui envoie des requetes au processus dit serveur, et attend une réponse adapté

**serveur** : processus qui attend des requetes provenant de processus clients, les réalise ces requetes  envoie les resultats au clients.

**requete** : instruction, commande ou structure permettant les echange entre un client et un serveur

**socket** : comme un tube/pipe, avec en plus la communication distante et le choix de protocole de communicaiton , (socket = prise en anglais) c'est un extrémité du canal de communication bidirectionnel entre deux processus.

### Type de socket:

#### Domaine

**IPv4 (PF_INET)**
**IPv6 (PF_INET6)** 

#### Format de transmission de données

**Datagramme (SOCK_DGRAM)** : un message est expédié comme un paquet bien délimité et est reçu entièrement en une seule fois.

**Stream (SOCK_STREAM)** : un message est expédié/reçu comme un flot continu de caractères. Si l'expéditeur envoie plusieurs messages, le destinataire pourra lire ces messges en une fois, en plusieurs fois ou caractère par caractère. Les limites des messages ne sont pas définis. 

#### Mode de connexion

**Connecté** : la transmission de messages est précédée par une phase de connexion avec une autre socket. Une socket en mode connecté est donc utilisée pour communiquer de façon exclusive avec une suel autre socket. Nous parlons de circuit ou canal vitruel établi entre les deux sockets (comme un communication téléphonique)

**Non connecté** : la destination d'un message à émettre via un socket en mode non connectée n'est pas nécessairement la même que celle du message suivant. A chaque émission, une adresse de destination doit être spécifiée (comme un communication postal)

#### Protocole

| TCP                    | UDP                  |
|------------------------|----------------------|
| fiable                 | non fiable           |
| ordre garanti          | ordre non garanti    |
| duplication impossible | duplication possible |
| mode connecté          | mode non connecté    |

Pour pouvoir ce connecter et envoyer des données à une socket distante, il faut connaitre son adresse (IP, Port)

- Associer une adresse à un socket est appelée : **nommage**

Pour faire cela on peut soit : 

- choisir manuellement une IP et un PORT, le système verifiera leur disponibilité
- laisser le système choisir une ou des adresses.

port a utiliser sont > 1024

Action                            | Fonction
--------------------------------- | ----------------------------------------------
Creation                          | `socket(domain, type, protocol)`
Nommage                           | `bind(sockfd, *addr, addrlen)`
Emission en mode non connecté     | `sendto(sockfd, *buf, len, flags, *dest_addr, addrlen)`
Reception en mode non connecté    | `recvfrom(sockfd, *buf, len, flags, *src_addr, *addrlen)`
Activer le mode connecté          | `listen(sockfd, backlog)`
Demander une connexion            | `connect(sockfd, *addr, addrlen)`
Accepter une connexion            | `accept(sockfd, *addr, *addrlen)`
Emission en mode connecté         | `send(sockfd, buf, len, flags)`
Reception en mode connecté        | `recv(sockfd, *buf, len, flags)`
Fermerture                        | `close(sockfd)`

`connect(sockfd, *addr, addrlen)`

```c

struct sockaddr_in {
    sa_family_t sin_family; // famille AF_INET
    in_port_t sin_port;     // numéro de port au format réseau
    struct in_addr sin_addr // structure d'adresse IP
}; 

struct in_addr {
    uint32_t s_addr; // adresse IP au format réseau
};

struct sockaddr_in6; // Adresse IPv6 + port

struct sockaddr {          // Adresse generique
    sa_family_t sa_family; // famille d'adresses, AF_XXX
    char sa_data[14];      // 14 octets pour l'IP + port
}

```

TCP gerer cas ou fonction send et recv renvoyent le mauvais nombre de d'octets


serveur iteratif

serveur concurent multi thread


UDP vs TCP

- messages cours, requetes indépendantes et traitement court, UDP iteratif
- messages longs, requetes dépendantes ou traitement long, TCP concurrent

##### Mode de connexion [↺](#sommaire-)

### Remote controle procedure [↺](#sommaire-)

#### RPCL [↺](#sommaire-)

**Contrat PROG_NAME.x** :

```
program PROG_NAME {
    version PROG_VERSION {
        TYPE_RETOUR_1 PROC_NAME_1(TYPE_PARAM_1_1, ..., TYPE_PARAM_1_N) = 1
        ...
        TYPE_RETOUR_N PROC_NAME_N(TYPE_PARAM_N_1, ..., TYPE_PARAM_N_N) = n
    } = 1;
} = 1;
```

**Compiler PROG_NAME.x** : rpcgen -a PROG_NAME.x

- PROG_NAME_client.c → appels au serveur (à implementer)
- PROG_NAME_clnt.c   → conversion données client au format xdr
- PROG_NAME_server.c → procedure à executer (à implementer)
- PROG_NAME_svc.c    → converstion données serveur au format xdr
- PROG_NAME_xdr.c    → fonction pour convertir les données au format xdr
- PROG_NAME.h        → definitions des fonctions et structures de données
- Makefile.PROG_NAME → makefile pour l'application RPC

**PROG_NAME_server.c** :

```c
TYPE_RETOUR_1 *
PROC_NAME_1_svc(TYPE_PARAM_1_1 *var_1, ..., TYPE_PARAM_1_1 *var_n, struct svc_req *rqstp) {
    
    static TYPE_RETOUR_1 result;

    return &result;
}

...

TYPE_RETOUR_N *
PROC_NAME_1_svc(TYPE_PARAM_N_1 *var_1, ..., TYPE_PARAM_N_N *var_n,, struct svc_req *rqstp) {
    
    static TYPE_RETOUR_N result;

    return &result;
}
```

**PROG_NAME_client.c** :

```c
void PROG_NAME_1(char *host) {
    CLIENT *clnt;
    TYPE_RETOUR_1  *result_1;
    TYPE_PARAM_1_1  PROC_NAME_1_1_arg_1; ...; TYPE_PARAM_1_N  PROC_NAME_1_1_arg_n; 
    ...
    TYPE_RETOUR_N *result_n;
    TYPE_PARAM_N_1 PROC_NAME_N_1_arg; ...; TYPE_PARAM_N_N PROC_NAME_N_1_arg_n;


#ifndef DEBUG
    clnt = clnt_create (host, CONCOURS_PROG, CONCOURS_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        exit (1);
    }
#endif  /* DEBUG */

    result_1 = PROC_NAME_1_1(&PROC_NAME_1_1_arg_1, ..., &PROC_NAME_1_1_arg_n, clnt);
    if (result_1 == (TYPE_RETOUR_1 *) NULL) {
        clnt_perror (clnt, "call failed");
    }

    ...

    result_n = PROC_NAME_N_1(&PROC_NAME_N_1_arg, ..., &PROC_NAME_N_1_arg_n, clnt);
    if (result_n == (TYPE_RETOUR_N *) NULL) {
        clnt_perror (clnt, "call failed");
    }

#ifndef DEBUG
    clnt_destroy (clnt);
#endif   /* DEBUG */
}


int main (int argc, char *argv[]) {
    char *host;

    if (argc < 3) {
        printf ("usage: %s server_host\n", argv[0]);
        exit (1);
    }

    host = argv[1];
    PROG_NAME_1(host);
    
    exit(0);
}
```

#### MPI [↺](#sommaire-)

**Compiler**
mpicc main.c -o main

**Executé**
./main
mpirun -np 2 ./main

```c
#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

int main(int argc, char* argv[])
{
    int nbTask;
    int myRank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nbTask);
    MPI_Comm_size(MPI_COMM_WORLD, &myRank);

    printf("I am task %d out of %d\n", myRank, nbTask);

    MPI_Finalize();

    return 0;
}
```

MPI_Send(message, 13, MPI_CHAR, i, type, MPI_COMM_WOLD);
MPI_Recv(message, 20, MPI_CHAR, 0, type, MPI_COMM_WOLD, status);

MPI_Bcast => one to all
MPI_Reduce => all to one
MPI_Allreduce => all to all
MPI_Scatter => Distribute one to all
MPI_Gather => collect data all to one
MPI_Alltoall => Distribute all to all
MPI_Allgather => collect all to all

Syncronisation 

MPI_Barrier(MPI_COMM_WORLD)

#### OpenMP [↺](#sommaire-)

directives de compilation pour paralléliser un code sur une architecture SMP

```c
#pragma omp parallel for reduction(+:sum) // parallelise la somme avec un lock sur la variable sum
for (size_t i = 0 ; i < SIZE ; i++) {
    sum = sum + a[i] * b[i];
}
```

**Compiler**

gcc -fopenmp prog.c -o prog

**Executer**

Modifier l'environnement, OMP_NUM_THREADS, OMP_DYNAMIC, etc..
./prog

Action                         | Directive
-------------------------------|-------------------------
Creation d'une region parallel | parallel
Partage du travail             | for, sections, single
Synchronisation                | master, critical, atomic


## TP0 - Fork, tube

- architecture d'arbre avec fork
- commande pstree -p PID pour visualiser fork
- (à faire mais pas vitale) utilisation des tubes  

# TP1 - Thread, mutex, cond

- EX1 : comportement des thread
- EX2 : exclusion mutuel thread (mutex)
- EX3 (à refaire) : rendez-vous thread (variable conditionnelle) : chaque thread lancé par main incremente une variable puis s'endors, quand asser de thread dormes on les reveilles 
- EX4 (à refaire) : traitement synchronisé thread

# TP2 - File de messages

- EX1 : envoie et recuperation de message avec une certaines etiquette
- EX2 (à refaire en fork) : meme chose mais avec des processus concurrent (fork)

# TP3 - Memoire partagé

- EX1 (à refaire) : mise en place shm et parking_setup, parking_entrer, parking_sortir + semaphore pour acces concurrent
- EX2 (à refaire): rendez-vous processus
- EX3 (à refaire): traitement synchronisé thread

# TP4 - Communications client-serveur

- EX1 : communication UDP chat
- EX2 (à faire) : communication TCP traitement simultané clients

# TP5 - Remote procedure call

histoire de contrats à comprendre

- EX1 : client demande serveur calcul si nb est premier (prime.x)
- EX2 : tableau dinamique en rpc/xdr
- EX3 (à faire) : renvoyer liste dynamique