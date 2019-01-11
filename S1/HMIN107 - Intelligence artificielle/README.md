# HMIN107 - Intelligence artificielle

## Sommaire [↺](#sommaire-)

* [Informations](#informations-)
  + [Examens](#examens-)
  + [Ressources](#ressources-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : MAX(Exam, (50% Exam + 50% CC))
- CC Amphi : 22/10/2018
- TP Backtrack : 5/11/2018

### Ressources [↺](#sommaire-)

- [Cours et TPs](https://moodle.umontpellier.fr/course/view.php?id=1195)
- [Chaine de Wheeler Ruml](https://www.youtube.com/channel/UCdgFn-ezyEWGpnX6wsq4jMw)

## CSP (Constraint Satisfaction Problem) [↺](#sommaire-)

- Etats = valeurs d'un ensembles de variables
- Action = assignation/retraite de valeurs aux variables
- But = ensembles de contraintes que les valeurs doivent satisfaires

### Representation [↺](#sommaire-)

**Graphe binaire**

(exemple coloration de carte)

**Graphe n-aire**

(exemple n-reine ou puzzle arithmètique)

### Resolution [↺](#sommaire-)

#### Backtrack [↺](#sommaire-)

Pour resoudre un problème on prend une série de décisions, on fois la décision prise on peut se rendre que la décision n'est pas bonne, si c'est le cas, on retourne à la décision précedente pour voir si un peut en prendre une meilleure

```
Algorithme Backtracking(Assignation A, Network R) : Booléen 
Debut
	si |A| = |R.X| // Toutes les variables de R sont assignées
		afficher a
		retourner vrai
	x ← ChoixVariableNonAssignée(R.X, A)
	pour tout v ∈ Tri(R.D(x))
		si Consistant(A U {(x, v)}, R.C)
			si Backtracking(A U {(x, v)}, R)
				retourner vrai
	retourner faux
Fin
```

(exemple)

#### Heuristique [↺](#sommaire-)

##### Ordre des variables [↺](#sommaire-)

- lex : lexicographique
- rand : aléatoire
- min-dom : petit domaines en premier
- min-width : trie de la dernière a la première lex en selectionnant le degré min dans les variables non choisi 
- max-deg : ordre décroissant des degré (nb contraintes sur var) 
- max-card : ordre en selectionnant une variables connecté au plus grand nombre de variables deja selectionné

=> bon choix = min-dom + max-deg + lex

##### Arc-Consistance [↺](#sommaire-)

- Appliquable avant et pendant le backtrack
- Enlèves les valeurs qui n'ont pas de support dans les contraintes
- Evite de faire des assignation inutiles

```
Algorithme Revise(Variable x, Constraint c, Network R) : Booléen
Debut
	modif ← faux
	pour tout v ∈ R.D(x)
		si il n'existe pas t ∈ c tel que chaque valeur du tuple est dans le domaine courant de la variable x pour R.D
			supprimer v de R.D(x)
			modif ← vrai
	retourner modif
Fin

Algorithme ArcConsistance(Network R) : Booléen 
Debut
	Q ← {(x, c) | c ∈ R.C et x ∈ P(c)}
	tant que Q ≠ {}
		retirer (x, c) de Q
		si Revise(x, c, R)
			si R.D(x) = {}
				retourner faux
			sinon
				Q ← Q U {(x', c') | c' ∈ R.C, x' ∈ P(c'), c' ≠ c, x' ≠ x}
	retourner vrai
Fin
```
(exemple)

##### Forward Checking [↺](#sommaire-)

- Amelioration du backtrack, les variables on des domaines dynamiques
- Aprèes chaque assignation on élimine pour chaque domaine des variabless les valeurs qui violent des contraintes dans l'etat actuel des variables
- Pas besoin de l'arc-consistence, la consistance et maintenu toutes seule
- Si le domaine d'une des variables est vide après une assignation alors l'assignation ne menera pas a une solution et on backtrack puis essaye une autre assignation.

```
Algorithme Propage(Variable x, Valeur v, Assignation A) : Booléen
Accès en modification : Network R
Début
	pour tout c ∈ R.C tel que x ∈ P(c) et |P(c) - (var(A) U {x})| = 1
		pour tout w ∈ R.D(y) où y est l'unique variable de P(c) - (var(A) U {x})
			si il n'existe pas t ∈ c tel que t|_x = v et t|_y = w
				supprimer w de R.D(y)
	si R.D(y) = NULL
		retourner faux
	retourner vrai
Fin

Algorithme ForwardChecking(Assignation A, Network R) : Booléen
Debut
	si |A| = |R.X| // Toutes les variables de R sont assignées
		afficher a
		retourner vrai
	x ← ChoixVariableNonAssignée(R.X, A)
	D_old ← D
	pour tout v ∈ Tri(R.D(x))
		si Propage(x, v, A)
			si ForwardChecking(A U {(x, v)}, R)
				retourner vrai
		D ← D_old
	retourner faux
Fin
```
(exemple)

## SAT (Problème de satisfiabilité booléenne) [↺](#sommaire-)

### Resolution [↺](#sommaire-)

- cherche a prouvé la satisfiabilité d'un formule logique F sous forme normale conjonctive ((x1 v X2) ^ (x1 v x2) ^ ...)
- entré : Formule logique F sous forme normal conjonctive (clause)
- sortie : si F est satisfiables donne une solution sinon donne un sous-ensembles minimal de clause non satisfiables

#### DPLL [↺](#sommaire-)

- algorithme de type backtrack
- on assigne des variables en faisant sorte de satisfaire les clauses, si impossible alors backtrack
- pour chaque clause 
	- si un litteral est assigné à vrai alors la clause est satisfaite (plus besoin de la considerer)
	- si tous ses littéraux devenus faux, elle est insatisfaisable
	- si tous ses littéraux sauf un sont faux, alors clause unitaire on n'a qu'un seule moyen de la satisfaire
- On retarde le plus possible le moment de faire un choix, si clause devenu unitaire alors on la satisfait et on propage les effet de l'affectation (Propagation unitaire)

- Quand on fait un choix on utilise egalement la propagation unitaire pour propager les effet de l'assignation

```
Algorithme UnitPropagation(F)
Données : Formule logique F sous forme normale conjonctive
Resultat : Une simplification de la formule F dont les symboles inutiles ont été éliminés
Debut 
	tant que F contient une clause unitaire l
		supprimer toutes les clauses qui contiennent l // Ces clauses sont satisfaites
		supprimer ¬l de toutes les clauses de F // Ceci peut vider une clause, ce qui provoquera un echec
	retourner F
Fin

Algorithme DPLL(F) // Davis Putnam Logemann Loveland
Données : Formule logique F sous forme normale conjonctive, sol un ensemble vide de solution
Resultat : Vrai si toutes les clauses sont satisfaites, faux sinon
Debut
	si F = NULL
		retourner vrai // Toutes les clauses sont satisfaites
	si F a une clause vide
		retourner faux // Une clause ne peut être satisfaite
	si F a une clause unitaire
		retourner DPLL(UnitPropagation(F))
	choisir une variable non assignée x
	retourner DPLL(F U {x}) ou DPLL(F U {¬x}) // Clause unitaire x ou ¬x ajoutée
fin
```
(exemple)

#### Heuristique [↺](#sommaire-)

MOMS (Maximum Occurences in Minimum Sized clauses) : on prend le literal qui apparait le plus souvent dans les clauses les plus petites mais non unitaires 

#### Reduction polynomiale [↺](#sommaire-)

##### SAT → CSP [↺](#sommaire-)

- symboles de F → variables de X
- ∀ x ∈ X, D(x) = {1,0}
- Clauses → Contraintes

(exemple)

##### CSP → SAT [↺](#sommaire-)

**traduction des variables et des domaines**
- (variable x peut prendre la valeur v → symbole xv) ↔ F = { xv | x ∈ X et v ∈ D(x) } 
- ∀ x ∈ X avec D(x) = { v_1, ..., v_n } on doit obtenir ces clauses :
	- (xv_1 v ... v xv_n ) ↔ x prend au moins une valeur de D(x)
	- (!xv_i v !xv_j) avec i < j et 1 ≤ i,j ≤ n ↔ x prend au plus une valeur de D(x)

**traduction des contraintes**
- pour chaque s contrainte c portant sur des variables (x_1, ...,  x_k) on obtiendra un ensembles de clauses de taille k, qu'on construira a partir des tuples interdits par la contrainte
- si le tuple (v_1, ..., v_k) est interdit par la contrainte : 
	- on obtiendra ¬(x_1v_1 ∧ ... ∧ x_kv_k ) ce qui est equivalent a la clause (¬ x_1v_1 ∨ ... ∨ ¬ x_kv_k)

(exemple)

## Système à base de règles d'ordre 0 (Logique des propositions) [↺](#sommaire-)

- base de connaissance
	- base de faits : observation factuelle sur une situation précise
	- base de règles : connaissances générales sur un domaine d'application 

- règle R : H -> C applicable à BF si H inclus dans BF
- application utile si C ∉ BF
- appliquer R à BF consiste à ajouter C à BF
- BF est saturé si aucune application d'une règle de BR à BF n'est utile

monde ouvert : on ne sait pas tout sur le monde, ce qui ne découle pas de la base est inconnue

monde clot : on sait tout sur le monde ce qui ne découle pas de la base est considéré comme faux

### Resolution [↺](#sommaire-)

#### Forward Chaining [↺](#sommaire-)

- principe : applique les règles sur les faits pour generer des nouveaux faits
- la base de faits est saturée si on ne peut plus produire de nouveau faits

```
Algorithme ForwardChaining(K) // d'ordre 0
Données : K = (BF, BR)
Resultat : BF saturée par BR
Debut
	à_traiter ← BF
	pour toute règle R ∈ BR
		compteur(R) ← |hypothèse(R)| // Nombre de littéraux de l'hypothèse de R
	tant que à_traiter ≠ NULL
		retirer F de à_traiter
		pour toute règle R : H → C ∈ BR telle que F ∈ H
			décrémenter compteur(R)
			si compteur(R) = 0 // R est applicable
				si C ∉ (BF U à_traiter) // l'application de R est utile
					ajouter C à à_traiter
					ajouter C à BF
Fin
```

(exemple)

#### Backward Chaining [↺](#sommaire-)

- principe : prouver un but en remontant le long des règles
- le but initial est prouvé quand on arrive à une liste de buts vide

```
Algorithme BackwardChaining(K, Q, L)
Données : K = (BF, BR), Q un atome, L un ensemble d'atomes (à ne pas générer)
Resultat : Vrai ssi Q est prouvable
Début
	si Q ∈ BF
		retourner vrai
	pour toute règle R = H1 ∧ ... ∧ Hn → Q de BR
		si aucun des H1 ... Hn n'appartient à L // Sinon on va boucler
			i ← 1
			tant que i ≤ n et BackwardChaining(K, Hi, L U {Q})
				i++
			si i > n
				retourner vrai // Hypothèse de R prouvée, donc Q aussi
	retourner faux // Aucun des faits et aucune des règles ne permet de prouver Q
Fin
```

### Representation [↺](#sommaire-)

Graphe ET OU

## Système à base de règles d'ordre 1 (Logique des predicats) [↺](#sommaire-)

### Homomorphisme [↺](#sommaire-)

Un homomorphisme H d'un requete Q dans BF est une substitution des variables de Q par des constantes de BF tel que h(Q) inclus dans BF

### Forward Chaining [↺](#sommaire-)

```
Algorithme ForwardChaining(K) // d'ordre 1
Données : K = (BF, BR)
Resultat : BF saturée par BR
Debut
	fin ← faux
	tant que non fin
		nouv_faits ← {}
		pour toute règle R : H → C ∈ BR
			pour tout (nouvel) homomorphisme S de H dans BF
				si S(C) ∈ (BF U nouv_faits)
					ajouter S(C) à nouv_faits
		si nouv_faits = {}
			fin ← vrai
		sinon
			ajouter les éléments de nouv_fais à BF
Fin
```
### Reduction polynomiale [↺](#sommaire-)

#### HOM → CSP [↺](#sommaire-)

HOM:
A1 = { p(x, y), p(y, z), q(z, x) }
A2 = { p(a, b), p(b, a), p(a, c), q(b, b), q(a, c), q(b, c) }
Qu'elles sont les Homomorphismes de A1 dans A2 ?

CSP:
Réseau(X, D, C)
Ce réseau est-il (globalement) consistant ?

X : variables de A1
D : constantes de A2
C : les atomes de A1 fournissent les contraintes, les atomes de A2 fournissent les tuples de ces contraintes

transformations:
si p(x, x) avec x variable
	p(x, y) et x = y (y nouvelle variable)

si p(x, a) avec x variable et a constante
	p(x, y) avec Domaine(y) = {a} (y nouvelle variable)

#### CSP → HOM [↺](#sommaire-)

A1 ← ensemble des variables de X
A2 ← ensemble des valeurs des variables x ∈ X

A1 = { Ci(x1, ..., xk) | Ci ∈ C et porte sur (x1, ..., ck)}
A2 = { Ci(a1, ..., ak) | Ci ∈ C et (a1, ..., ak) est dans la définition de ci}






#### Negation Monde ouvert/clos

Mecanisme de chaînage avant/arrière est en
	- **Adéquation** (ou correction) : si l'atome A est produit par un mecanisme de chaînage alors A est conséquence logique de la base de connaissances
	pour tout atome A, si A est dans BF*, A est consequence logique de K
	- **Completude** : si l'atome A est conséquence logique de la base alors un mecanisme de chaînage le produit forcement
		pour tout atome A, si A est consequence de K alors A est dans BF*
Règle negative : 

**Adéquation** : il ne fait que des déduction
	- Chainage avant : si ∀ A, A ∈ BF* alors BF, BR |= A  
	- Chainage arrière : si ∀ A, A est prouvé alors BF, BR |= A

**Complétude** : il fait toute les déduction
	- Chainage avant : si ∀ A, BF, BR |= A alors A ∈ BF*  
	- Chainage avant : si ∀ A, BF, BR |= A alors A est prouvé  

**règle conjonctive positive** : adéquation et completude 
**règle conjonctive (pas forcement positive)** : adéquation mais pas complétude 

BF = {A}
R1 : A, not B → C
R1 : A → B

Si on applique R1 avant R2, on obtient BF* = {A, C, B}. Mais ceci est problématique car on s'est servi de l'absence de B pour produire C, alors qu'on a finalement B présent. Le résultat ovulu est BF* = {A, B}

Si on utilise not B pour inférer quelque chose il faut être sur qu'on ne vas pas produire B par la suite

Il faut ajouter des règles qui assurent que :

	- la négation est utilisée correctement : l'application d'une règle dont l'hypothèse contient not B intervient toujours après les applications des règles qui produisent B
	- le résultat de la saturation est unique

**Règles semi-positif** : seuls les symboles qui n'apparaissent pas en conclusion de règles peuvent être niés. L'ordre des règles n'a alors aucune importance.

**Règles stratifié** : l'idée est de mettre les règles dans un ordre qui assure que l'orsqu'on utillise un litéral négatif dans une hypothèse de règle, on ne peut plus produire ce littéral en positif. A chaque symbole p on asscie un entier α(p). Les règles sont ensuite rangées en strates suivant le numéro α associé à leur conclusion. On éxécute les règles en marche avant par ordre croissant des strates : à l'étape i, on sature la base de faits calculée à l'étape i - 1 avec la règles de la strate i.

un ensemble de règles est stratifiable ssi son graphe de précedence n'admet aucun circuit avec un arc négatif



