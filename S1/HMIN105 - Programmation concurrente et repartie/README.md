# HMIN105M - Programmation concurrente et repartie

## Sommaire

* [Informations](#informations)
  + [Examens](#examens)
  + [Ressources](#ressources)
* [Les Threads](#les-threads)
  + [Les Mutex](#les-mutex)
  + [Les Conditions](#les-conditions)
  + [Thread vs Fork](#thread-vs-fork)

## Informations

### Examens
	
- Note final : 60% Exam + 40% TP
- TP noté multithreading : 23/10/2018
- Projet client-serveur + IPC + multithreading : 04/12/2018

### Ressources

- [Cours et TPs](https://moodle.umontpellier.fr/course/view.php?id=675)
- [System Programming Wiki](https://github.com/angrave/SystemProgramming/wiki)

## Les Threads

Tous les objets et fonctions manipulés sont definis dans pthread.h sous les formes :

- pthread_objet_t
- pthread_objet_opération()

Action         | Fonction         | Remarque                  
---------------|------------------|---------------------------
Creation       | pthread_create() | Créée un thread           
Fin/Abandon    | pthread_exit()   | Different de exit()       
Identification | pthread_self()   | Resultat de type pthread_t
Egalité        | pthread_equal()  | Portabilité : eviter =    
Attente        | pthread_join()   | Portabilité : eviter =    

L'option -lpthread ou -pthread peut être necessaire à la compilation.

```c
/**
 * Cette fonction demarre l'execution d'un nouveau thread en parallèle 
 * avec celui qui l'a appelé mais dans le même processus.
 * 
 * @param  thread   Identité obtenue en résultat
 * @param  attr     NULL pour commencer
 * @param  function Fonction à executer
 * @param  arg      Argument(s) à passer a la fonction
 * @return 0 si réussite, != 0 sinon
 */
int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*function)(void*), void* arg);

/**
 * Met fin à l'appel du thread.
 * 
 * @param  retval valeur à retourner au parent
 */
void pthread_exit(void* retval);

/**
 * @return  Identité du thread appelant.
 */
pthread_t pthread_self(void);

/**
 * Compare les thread t1 et t2.
 * 
 * @param  t1 
 * @param  t2 
 * @return  valeur != 0 si t1 = t2, 0 sinon 
 */
int pthread_equal(pthread_t t1, pthread_t t2);

/**
 * Attend que le thread spécifier en paramètre ce termine.
 * 
 * @param  thread Thread à attendre
 * @param  retval Valeur retourner par le thread qui ce termine
 * @return  0 si le thread ce termine, != 0 sinon
 */
int pthread_join(pthread_t thread, void** retval);
```

**Attention** la fonction exit() termine le processus même si executer dans un processus fils.

### Les Mutex

Une fonction manipulant un mutex est de la forme, pthread_mutex_fonction().

Fonction                | Resultat                                             
------------------------|------------------------------------------------------
pthread_mutex_init()    | Verrou créée, etat libre                             
pthread_mutex_lock()    | Verrouillage                                         
pthread_mutex_trylock() | Verrouillage si etat libre, sinon erreur sans blocage
pthread_mutex_unlock()  | Deverrouillage, etat libre                           
pthread_mutex_destroy() | Destruction                                          

Une initialisation plus simple : 

```c
pthread_mutex_t verrou = PHTREAD_MUTEX_INITIALIZER;
```

**Règle :** Tout accès à une variable accessible en lecture par un thread et en écriture par un autre doit être protégé.

- Un seul mutex peut protéger plusieurs variables, mais pas l’inverse.
- Les opérations ...lock() et ...unlock() sont atomiques, mais pas la portion de code qui se trouve entre les deux.
- Cette portion de code est appelée section critique.
- Si un thread est dans une section critique, il doit être garanti qu'aucun autre thread n'y sois pas simultanément.

### Les Conditions

Sur une variable conditionnelle c et un verrou v, on peut effectuer les actions suivantes :

Fonction                               | Action                                               
---------------------------------------|------------------------------------------------------
pthread_cond_init(&c)                  | Créée la variable conditionnelle c                   
pthread_cond_destroy(&c)               | Destruction                                          
pthread_cond_wait(&c, &v)              | Bloque l'appelant et rend le verrou de façon atomique
pthread_cond_timedwait(&c, &v, &delai) | Wait avec un delai                                   
pthread_cond_broadcast(&c)             | Libère tous les threads boqués                       
pthread_cond_signal(&c)                | Libère un seul thread                                

**Remarque :** Toutes ces fonctions retournent 0 en cas de succès et un résultat non-nul en cas d’erreur, accompagné d’un code d’erreur.

```c
/**
 * @param  cond variable conditionnelle à créée
 * @param  attr NULL par default
 */
int pthread_cond_init(pthread_cond_t* cond,  pthread_condattr_t* attr); 

/**
 * @param  cond variable conditionnelle à detruire
 */
int pthread_cond_destroy(pthread_cond_t* cond);

/**
 * Appel bloquant qui de façon atomique déverouille verrou et
 * attend que la condition cond soit annoncée (par un autre thread).
 */
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* verrou);

/**
 * Provoque le réveil d’un thread mis en attente par la condition cond.
 */
int pthread_cond_signal(pthread_cond_t* cond);

/**
 * Provoque le réveil de tous les threads mis en attendant la condition cond.
 */
int pthread_cond_broadcast(pthread_cond_t* cond) ;
```

**Important :**

- La primitive suppose que le thread appelant a obtenu précédemment le verrouillage de verrou.
- Plusieurs threads peuvent être débloqués. Il est donc utile de tester à nouveau le prédicat après réveil
- Lors du réveil, les threads réveillés vont obtenir tour à tour automatiquement le verrouillage du verrou.

**Choix signal ou broadcast :**

- Utiliser signal seulement si on est certain que n’importe quelle tâche en attente peut faire le travail requis et qu’il est indispensable de réveiller une seule tâche.
- Lorsqu’une variable conditionnelle est utilisée pour plusieurs prédicats, signal est à prohiber.
- En cas de doute, utiliser broadcast.

**Déverrouiller après ou avant l’annonce :**

- Après engendre qu’un thread réveillé ne pourra pas obtenir le verrouillage immédiatement car le verrou est toujours indisponible. Donc le thread réveillé devra se bloquer temporairement.
- Avant peut être plus efficace, mais il se peut aussi qu’un thread Tz non (encore) en attente obtienne le verrouillage. Il n’y a pas d’équité, alors que le thread réveillé Ta peut être plus prioritaire (Tz moins prioritaire a obtenu le verrouillage alors que Ta, en attente de l'annonce, ne pouvait l'obtenir).

### Thread vs Fork

Fork                                            | Thread
------------------------------------------------|---------------------------------------------------------------------
Processus lourd                                 | Processus leger
Changement de contexte couteux                  | Partie d'un processus
Espace d'adressage de processus non partageable | Peu partager des données en memoire avec d'autres threads
Outils de synchronisation difficiles            | Permet d'executer plusieurs unités d'execution de manière asynchrone
Plus difficile à implementer                    | Execute une fonction
