# HMIN107 - Intelligence artificielle

## Sommaire [↺](#sommaire-)

* [Informations](#informations-)
  + [Examens](#examens-)
  + [Ressources](#ressources-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : MAX(Exam, (50% Exam + 50% CC))
- CC Amphi : 22/10/2018
- TP Backtrack : 5/10/2018

### Ressources [↺](#sommaire-)

- [Cours et TPs](https://moodle.umontpellier.fr/course/view.php?id=1195)

## Definitions

**Probleme :** Collection d'information que l'agent utilise pour décider quelle(s) action(s) accomplir

**Definition d'un Problème :**

- Identification d'un **état initial** (choix d'un langage de description d'états du problème)
- Identification des **actions possibles** par définition d'opérateurs de changements d'état (definition de l'ensemble des états possibles du problème).

**Espace des états/de recherche d'un problème :** Ensemble des états atteignables depuis l'état initial par n'importe quelle séquence d'actions (representable par un graphe orienté).

**Solution d'un Problème :** séquence d'actions permettant de passer de l'état initial vers un état but (chemin orienté).

## Formalisation d'un problème

| Type    | Composants              | Opérations                                    |
|         |                         |                                               |
| Problem | InitialState, Operators | GoalTest(State), PathCost(SequenceOfOperator) |

**Problèmes à états simple :**

- On connaît l’état dans lequel on est
- On connaît précisément l’effet des actions
- On peut calculer à tout moment l’état dans lequel on se trouvera après une action

**Problèmes à états multiples :**

- On ne sait pas exactement dans quel état on se trouve (seulement un ensemble d’états possibles)
- On ne connaît pas précisément l’effet des actions
- On ne peut que caractériser par un ensemble d’états la situation où l’on est

## Algorithme de Résolution

- Il s'agit donc d'effectuer une rechercher à travers l'espace des états
- L'idée est de maintenir et d'étendre un ensemble de solutions partielles : des séquences d'actions qui amènent à des états intermédiaires "plus proche" de l'état but

## Génération des Solutions Partielles

**Un cycle en 3 phases**

1. Tester si l'état actuel est un état but
2. Générer un nouvel ensemble d'états à partir de l'état actuel et des actions possibles
3. Sélectionner un des états générés (à cette étapes ou précédemment) et recommencer

Le choix de l'état à considérer (la sélection) est déterminé par la stratégie de recherche.

## Processus de Résolution

- Constructoin d'un arbre de recherche qui se superpose à l'espace des états du problème.

- Chaque noeud correspond à l'état initial ou un devellopement du sommet parent par un des opérateurs du problème

## Arbre de Recherche

- Racine correspond à l'état initial
- Feuilles sont soit des noeuds associés à des états sans action soit des noeuds non encore développés
- Un chemin est une séquence de sommets partant du sommet à une feuille

## Noeuds d'un Arbre de Recherche

| Composants                            | Details                                                                                  |
|                                       |                                                                                          |
| State                                 | Etat danss l'espace des états auquel le noeud correspond                                 |
| ParentNode                            | Le noeud ayant généré ce noeud                                                           |
| Operator                              | Opérateur utilisé pour générer ce noeud                                                  |
| Depth                                 | Le nombre de noeuds - 1 du chemin de la racine à ce noeud                                |
| PathCost                              | Le coût de ce chemin                                                                     |

| Operations                            | Details                                                                                  |
|                                       |                                                                                          |
| Node MakeNode(State)                  | Fabrique un noeud à partir d'un état (utilisé pour l'état initial)                       |
| SetOfNode Expand(Node, SetOfOperator) | Calcule l'ensemble des noeuds générés par l'application des opérateurs au noeud spécifié |

**Frontière :** Ensemble des noeuds non encore développés (explorés) de l'arbre de recherche

## Queue d'un Arbre de Recherche

| Operations                  | Details                                                          |
|                             |                                                                  |
| Queue MakeQueue(Node)       | Construit une liste à un  noeud                                  |
| Bool Empty?(Queue)          | Retourne vrai si la liste est vide                               |
| Node RemoveFront(Queue)     | Extrait le noeud en tête                                         |
| QueuingFn(SetOfNode, Queue) | Insère des noeuds dans la liste selon une stratégie particulière |

## Fonction Générale de Recherche

```
GeneralSearch(Problem p, QueuingFn strategy) : Node
	Queue frontier := MakeQueue(MakeNode(p.InitialState))
	do
		if Empty?(frontier) then return null
		Node n := RemoveFront(frontier)
		if GoalTest(n.State) then return n
		frontier := strategy(Expand(n, p.Operators), frontier)
	end
end
```
## Performance d'une Stratégie de Résolution

- **Complétude** : La technique de résolution marche t'elle dans tous les cas
- **Optimalité** : La technique de résolution trouve t'elle une solution de coût minimal
- **Complexité** : La technique est-elle coûteuse (en **temps** ?, en **mémoire** ?)

# Strategie de Recherche Simple

## Recherche en largeur

- Trouve une solution la plus proche de la racine
- Optimale seulement si
	- le critère d'optimalité diminue avec le nombre d'opérations effectués
	- et que toutes les Opérations ont le même coût

**Complexité** : 

- Soit p la profondeur de la solution trouvé
- soit b le facteur de branchement (nombre max de noeuds générés à chaque expansion)

- Les complexité temporelle et spatiale sont bornées par O(p^(b + 1))

## Dijkstra (Recherche par Coût)

**Complétude** :

- Complète si les coûts sont positifs

**Optimalité** :

- Optimale si le PathCost augmente avec le nombre d’opérateurs: ∀ n PathCost (successeur (n)) ≥ PathCost(n)
- Quand le PathCost est la somme du coût des opérateurs, elle est optimale si les opérateurs n’ont pas de coût négatif

**Complexité spatiale et temporelle** :

- Soit C le coût de la solution, p le coût de l’action min, la profondeur maximum de l’arbre de recherche sera C/p, soit une complexité de O(b^C/p )

## Recherche en Profondeur

**Complétude** :

- On insère les nœuds développés en tête de liste ce qui peut conduire à développer des branches infinies ou créer des cycles

**Optimalité** :

- Non Optimale car elle retourne la première solution rencontrée sans
aucune corrélation avec un critère de coût

**Complexité spatiale et temporelle** :

- Peu coûteuse en mémoire car on ne mémorise qu’un chemin (plus les nœuds frontières) et non l’arbre entier

- Soit m la profondeur maximale de l’espace de recherche et b le facteur de branchement: 
	- Complexité temporelle : O(b^m), mais elle peut être rapide en pratique si le problème possède beaucoup de solutions
	- Complexité spatiale : O(mb)

# Strategie de Recherche heuristique

- Cela revient à considérer que l'on dispose d'une fonction d'évaluation d'état qui retourne le coût d'un chemin d'un état à l'état but le plus proche (Estimation)

# CSP : Constraint Satisfaction Problem

Un CSP est un type de problème particulier

- Les états sont définis par les **valeurs** d'un ensemble de variables
- Les variables sont caractérisées par un **domaine** de valeurs possibles associé à chaque variable
- Les actions sont des **assignations** de valeurs aux variables et/ou des **retraits** de valeurs possibles aux domaines
- La fonction de test de but est un ensemble de **contraintes** auquel les valeurs des variables doivent satisfaire

- Un CSP est le problème défini par :
	- Instance : un réseau de contraintes R=(X,D,C)
	- Question : Sol(R) ≠ ∅ (R a-t-il une solution ?)

- CSP est NP-complet

## CSP vs Problème Standard

- Les CSP définissent une structure particulière pour les états : un ensemble de variables partiellement valuées, et une structure particulière pour la fonction but : **un ensemble de contraintes**
- Les algorithmes de recherche d’une solution peuvent tirer parti de cette structuration
	- On passe d’une vision exploration d’un ensemble d’états : d’un état initial à un état but
	- À une vision **résolution pas à pas d’un problème** : de l’étape où aucune variable n’est assignée à l’étape où toutes les variables sont assignées
- Les techniques de résolution dépendent de la nature des domaines et contraintes

## Réseau de Contraintes

- Un réseau de contraintes est un triplet (X,D,C)
	- X = {X1 ,X2 ,... ,Xn} est un ensemble fini de variables
	- D, le domaine de X, est une fonction qui associe à chaque variable Xi un ensemble fini de valeurs D(Xi) (le domaine de la variable Xi).
	- C = {C1 ,... ,Cm} est un ensemble de contraintes

- Chaque contrainte ci est une relation (i.e. un ensemble de tuples de valeurs)
	- définie sur un sous-ensemble ordonné des variables de X, appelé sa portée et notée* P(Ci) = <Xi1 ,... ,Xik> où k est **l’arité**.
	- Ci désigne donc l’ensemble des tuples de valeurs autorisés par la contrainte (généralement Ci ⊆ D(Xi1)X...XD(Xik))

## Assignation Partielle et Complète

- Une **assignation** A sur un sous-ensemble X’ des variables de X est une application qui associe à chaque variable x de X’ une valeur de son domaine D(x)
	- si X’⊂ X on parle **d’assignation partielle**
	- si X’= X on parle **d’assignation complète**
- On note :
	- **var(A)** le domaine de A (les variables ayant une image par A)
	- **A[t]** l’application de A aux variables de t (t étant un tuple de variables distinctes de var(A))

## Consistance et Solution


- Une assignation A **viole une contrainte** c ssi
	1. **P(c) ⊆ var(A)** (toutes les variables de c ont une image par A)
	2. **A[P(c)] ∉ c** (le tuple de valeur défini par A pour les variables de c n’est pas autorisé)

- A est **localement consistante** si A ne viole aucune contrainte de C Une solution est une assignation complète qui ne viole aucune contrainte de C
- L’ensemble des **solutions** d’un réseau de contraintes R = (X,D,C)
est noté Sol(R)

## Résolution d’un CSP : algo naïf

- L’état initial est l’état dans lequel aucune variable n’est
assignée : A = ∅
- Une action consiste à choisir une variable non assignée et à lui
associer une valeur de son domaine : A <- A U {(x i ,v)} où v ∈ D(xi)
	- Le facteur de branchement est borné par la somme des tailles des domaines
	- La profondeur de l’arbre de recherche est naturellement bornée par le nombre de variables
- Lorsque toutes les variables sont assignées, la fonction de test de but vérifie si les contraintes sont satisfaites ou pas
	- A est il une solution de R ? On peut donc utiliser un algorithme de recherche en profondeur -> O((Σi|Di|)^|X|)

## Backtracking Algorithm

- L’algorithme prend en entrée
	- Un réseau de contraintes
	- Une assignation partielle localement consistante
- On teste à chaque extension de l’assignation partielle courante si elle ne viole pas de contraintes
- Si viol, on « backtrack » immédiatement : on revient sur les choix précédents de valeur
- Si non, on continue à développer la branche

```
BacktrackingSearch(Network R = (X,D,C)) = BT({}, R)

Fonction BT(Assignation A, Network R) : Booléen
Début
	si |A| = |R.X| alors
		afficher a
		retourner true
	finsi
	
	x := ChoixVariableNonAssignée(R.X, A)
	
	pour tout v ∈ Tri(R.D(x)) faire
		si Consistant(A∪{(x,v)}, R.C) alors
			si BT(A∪{(x,v)}, R) alors retourner true 
			finsi
		finsi
	finpour
	
	retourner false
Fin
```
# Probleme

**Definition**

- Identifier **l'état initial**
- Identifier les **actions possibles**
- Identifier **l'état but**

**Resolution**

- Trouver un chemin qui va de l'état initial à l'état but

**Solution**

- Sequences d'action qui permettent de passer de l'état initial à l'état but

# Algorithme de résolution

- Correspond a une simple recherche de l'état but à travers l'espace des états
- L'idée est de maintenir et d'etendre un ensemble de solution partielles qui se rapprochent de l'état but

**Géneration des solutions partielles**

1. Tester si l'état actuel est un état but
2. Générer un nouvel ensemble d'états à partir de l'état actuel et des action possibles
3. **Sélectionner** un des états générés et recommencer

lors de la selection plusieurs strategie sont possible selon le problème

**Processus de resolution**

- creation d'un arbre de recherche
- racine = etat initial
- feuilles = etats sans action ou noeuds non encore developper

# Strategie de resolution

**Evaluation des performance**

- Completude
- Optimalité
- Complexité 
	- En temps
	- En memoire

**Recherche en largeur**

**principe** : Les noeuds de profondeur d sont développés avant ceux de profondeur d+1.

- Completude : si un solution existe elle sera trouver
- Optimalité : trouve une solution la plus proche de la racine
- Complexité en temps et memoire:
	- Soit d la profondeur à laquelle la solution est trouvée
	- Soit b le nombre max de noeuds générés à chaque expansion
		- Complexité **bornées par O(b^(d+1))**

**Recherche par cout**

**principe** : On sélectionne parmi les noeuds frontière le noeud dont le coût associé à son chemin depuis la racine est le moins élevé.

- Completude : si les coûts sont positifs
- Optimale : si PathCost augmente avec le nombre d'operateurs et pas de coût negatif
- Complexité : 
	- Soit c le cout de la solution
	- Soit p le cout de l'action min
		- La profondeur maximum sera de c/p
		- La complexité en **O(b^(c/p))**

**Recherche en profondeur**

**principe** : On développe toujours un des noeuds les plus profonds et on ne remonte que lorsqu'on tombe sur un noeud non but et non devellopable.

- Complexité :
	- Soit m la profondeur max de l'espace de recherche
	- Soit b le facteur de branchement
		- Complexité temporelle : **O(bᵐ)**
		- Complexité spatiale : **O(mb)**

- Non Complete : On insère les noeuds développer en tete de liste
	- Peut creer des cycles ou des branches infinies

- Non optimale : retourne la première solution rencontrée sans correlation de 

**amelioration**

- completude : si on connait la profondeur max de la solution on peut appliquer borne max a l'algorithme pour ne pas explorer les branches infinies

- supression des etats répéter : 
	- Ne pas retourner à l'état d'ou l'on viens O(1)
	- Ne pas creer de chemin avec des cycles en comparent tout le noeuds de puis la racine O(d)
	- Ne pas generer d'état déjà généré avec un stockage de tout les etats visité O(s) avec s taille de l'espace des etats

# CSP : Constraint Satisfaction Problem

- Etats definis par les valeurs d'un ensemble de variables qui sont caractérisées par un domaine de valeurs possible
- Actions sont des assignation de valeurs aux variables et/ou des retraits de valeurs possibles aux domaines


- Etats : Ensemble de variables partiellement valuées
- Fonction but : Ensemble de contraintes

**Resau de contraintes**

- Triplet (X, D, C)
	- X = {x_1, x_2, ..., x_n} est un ensemble finis de variables
	- D, le domaine de X est une fonction qui associe à chaque variable x_i un ensemble fini de valeurs D(x_i)
	- C = {c_1, c_2, ..., c_n} est un ensemble de contraintes

Chaque contrainte c_i est une relation (un ensemble de tuples de)

**graphe**

**Resolution CSP**

**heuristique**

choisir la variable ayant le plut petit domaine
