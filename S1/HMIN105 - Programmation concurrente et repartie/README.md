# HMIN105M - Programmation concurrente et repartie

## Sommaire [↺](#sommaire-)

- [Informations](#informations-)
  - [Examens](#examens-)
  - [Ressources](#ressources-)
- [Multithreading](#Multithreading-)
  - [Thread](#thread-)
  - [Mutex](#mutex-)
  - [Variable conditionnelle](#variable-conditionnelle-)
  - [Thread vs Fork](#thread-vs-fork-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : 60% Exam + 40% TP
- TP noté multithreading : 23/10/2018
- Projet client-serveur + IPC + multithreading : 04/12/2018

### Ressources [↺](#sommaire-)

- [Cours et TPs](https://moodle.umontpellier.fr/course/view.php?id=675)
- [System Programming Wiki](https://github.com/angrave/SystemProgramming/wiki)

## Multithreading [↺](#sommaire-)

Les objets et leurs operations en rapport avec le multithreading posix sont definis dans le fichier **pthread.h** sous la forme :

- **pthread_objet_t**
- **pthread_objet_opération()**

L'option **-lpthread** ou **-pthread** peut être necessaire à la compilation.

### Thread [↺](#sommaire-)

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

### Mutex [↺](#sommaire-)

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
- Si un thread est dans une **section critique**, il doit être garanti qu'aucun autre thread n'y sois pas simultanément.

### Variable conditionnelle [↺](#sommaire-)

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

### Thread vs Fork [↺](#sommaire-)

Fork                                            | Thread
----------------------------------------------- | --------------------------------------------------------------------
Processus lourd                                 | Processus leger
Changement de contexte couteux                  | Partie d'un processus
Espace d'adressage de processus non partageable | Peu partager des données en memoire avec d'autres threads
Outils de synchronisation difficiles            | Permet d'executer plusieurs unités d'execution de manière asynchrone
Plus difficile à implementer                    | Execute une fonction
