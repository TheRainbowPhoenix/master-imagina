# HLIN105 - Principes de la programmation concurrente et repartie

Moy(0,6 ECRIT, 0.4 TP)

# Forks vs Threads

**Fork :**

- Processus lourd
- Changement de contexte couteux
- Espace d'adressage de processus non partageable (Outils de communication commme les tubes, files, etc.. obligatoires)
- Outils de synchronisation difficiles

**Threads :**

- Processus leger
- Partie d'un processus
- Execute une fonction
- Peu partager des données en memoire avec d'autres threads
- Permet d'executer plusieurs unités d'execution de manière asynchrone

# Threads :

Tous les objets et fonctions manipulés sont definis dans pthread.h et sous les forme,

- pthread_objet_t
- pthread_objet_opération()

L'option -lpthread peut etre necessaire à la compilation.

| Action         | Fonction         | Remarque                   |
|----------------|------------------|----------------------------|
| Creation       | pthread_create() | Activité créée             |
| Fin            | pthread_exit()   | Different de exit          |
| Identification | pthread_self()   | Resultat de type pthread_t |
| Egalité        | pthread_equal()  | Portabilité : eviter =     |

**Creation :**

```c
int pthread_create(            // résultat 0 si réussite,  != 0 sinon
	pthread_t* idThread,       // identité obtenue en résultat
	pthread_attr_t* attributs, // NULL pour commencer
	void* (*fonction)(void *), // fonction à démarrer
	void* param);              // paramètre(s) à passer à la fonction
```

Cette fonction demarre l'execution d'un nouveau thread en parallèle avec celui qui l'a appelé mais dans le même processus.

**Abandon/fin :**

```c
void pthread_exit(void * retour);
```
Le paramètre retour est un résultat, (valeur de retour), pouvant être
consulté par un autre thread du même processus, attendant la fin de
celui qui vient d’abandonner.

**Identification :**

```c
pthread_t pthread_self(void);
```
Renvoie l’identité du thread appelant.

**Egalité :**

```c
int pthread_equal(pthread_t idT1, pthread_t idT2);
```
Renvoie une valeur non nulle si réussite, 0 si échec.

**Attention** la fonction exit() peu importe l'endroit ou elle est appelé termine tous les processus parent et fils.

**Attente fils:**

```c
int pthread_join(pthread_t idT, void **retourCible);
```
Permet au thread appelant d’attendre la fin de celui issu du même processus, identifié par idT. 
Renvoie 0 si réussite, 6 = 0 sinon.
L’appelant est bloqué si le thread idT n’est pas terminé. 
Il sera débloqué lorsque le thread idT aura fait pthread exit().


# Mutex

Une fonction manipulant un mutex est de la forme, pthread_mutex_fonction().

| Fonction                | Resultat                                              |
|-------------------------|-------------------------------------------------------|
| pthread_mutex_init()    | Verrou créée, etat libre                              |
| pthread_mutex_lock()    | Verrouillage                                          |
| pthread_mutex_trylock() | Verrouillage si etat libre, sinon erreur sans blocage |
| pthread_mutex_unlock()  | Deverrouillage, etat libre                            |
| pthread_mutex_destroy() | Destruction                                           |

Une initialisation plus simple : 

pthread_mutex_t verrou = PHTREAD_MUTEX_INITIALIZER;

**Règle :** Tout accès à une variable accessible en lecture par un thread
et en écriture par un autre doit être protégé.

- Un seul mutex peut protéger plusieurs variables, mais pas l’inverse.
- Les opérations ...lock() et ...unlock() sont atomiques, mais pas la portion de code qui se trouve entre les deux.
- Cette portion de code est appelée section critique.
- Si un thread est dans une section critique, il doit être garanti qu'aucun autre thread n'y sois simultanément.

# Thread condition

Sur une variable conditionnelle c et un verrou v, on peut effectuer les actions suivantes :

| Fonction                               | Action                                                |
|----------------------------------------|-------------------------------------------------------|
| pthread_cond_init(&c)                  | crée la variable conditionnelle c                     |
| pthread_cond_wait(&c, &v)              | Bloque l'appelant et rend le verrou de façon atomique |
| pthread_cond_timedwait(&c, &v, &delai) | Wait avec un delai                                    |
| pthread_cond_broadcast(&c)             | Libère tous les threads boqués                        |
| pthread_cond_signal(&c)                | Libère un seul thread                                 |
| pthread_cond_destroy(&c)               | Destruction                                           |

**Retour :** Toutes ces fonctions retournent 0 (zéro) en cas de succès et
un résultat non-nul en cas d’erreur, accompagné d’un code d’erreur.

**Creation :**

```c
int pthread_cond_init(         // résultat 0 si réussite, != 0 sinon
	pthread_cond_t* cond,      // variable conditionnelle à créer
	pthread_condattr_t* attr); // NULL par défaut
```

**Destruction :**

```c
int pthread cond destroy(pthread cond *cond);
```

Détruit la variable conditionnelle pointée.

**Attente non bornée :**
```c
int pthread_cond_wait(        // résultat 0 si réussite, 6 = 0 sinon
	pthread_cond_t* vcond,    // variable conditionnelle associée à l'evenement attendu
	pthread_mutex_t* verrou); // verrou d’accès à la donnée commune
```

Cette primitive réalise un appel bloquant, qui de façon atomique déverrouille verrou et attend que la condition vcond soit annoncée, forcément par un autre thread (voir ci-après pour l’annonce).

**Note :** La primitive suppose que le thread appelant a obtenu précédemment le verrouillage de verrou.

**Attention :** plusieurs threads peuvent être débloqués. Il est donc utile de tester à nouveau le prédicat après réveil

**Reveiller les processus en attente :**

```c
int pthread_cond_signal(pthread_cond_t* cond);
```
Provoque le réveil d’un seul thread. (Plus efficace)

```c
int pthread_cond_broadcast(pthread_cond_t* cond) ;
```

Provoque le réveil de tous les threads attendant la variable conditionnelle cond.

**Important :** Lors du réveil, les threads réveillés vont obtenir tour à tour
automatiquement le verrouillage du verrou.

**Choix signal ou broadcast**

- Utiliser signal seulement si on est certain que n’importe quelle tâche en attente peut faire le travail requis et qu’il est indispensable de réveiller une seule tâche.
- Lorsqu’une variable conditionnelle est utilisée pour plusieurs prédicats, signal est à prohiber.
- En cas de doute, utiliser broadcast.

**Déverrouiller après ou avant l’annonce**

- Après engendre qu’un thread réveillé ne pourra pas obtenir le verrouillage immédiatement car le verrou est toujours indisponible. Donc le thread réveillé devra se bloquer temporairement.
- Avant peut être plus efficace, mais il se peut aussi qu’un thread T z non (encore) en attente obtienne le verrouillage. Il n’y a pas d’équité, alors que le thread réveillé T a peut être plus prioritaire (T z moins prioritaire a obtenu le verrouillage alors que T a, en attente de l'annonce, ne pouvait l'obtenir).