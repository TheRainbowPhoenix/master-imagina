# EXERCICE 1

1. Un ensemble d'au plus 3 sequences. 
chaque séquence est ordonnée du plus haut au plus bas.

- Etat initial 	= { <C, A>, <B>, <> }
- Etat but 		= { <B, A, C>, <>, <> }

2. (espace de recherche)

3. (parcours en largeur)

93 noeuds générés, 31 explorés.

4. Avec le parcour en profondeur on ré-explore l'etat initial à l'infini.

5. Pour éviter de ré-etudier un état, au moment de la fonction expand on peut vérifier si l'etat appartient déjà à une table d'états deja générés.
k
(recherche en largeur améliorée)

13 noeuds générés, 11 explorés.

6. (recherche en profondeur améliorée)

11 noeuds générés, 9 explorés.

7. 
```
Node recherche_profondeur(Problem p){
	Node n <- make_node(p.initial_state);
	renvoyer recherche_prof_rec(n, p);
}

void recherche_prof_rec(Node nc, Problem p, List s){

	Node rep;

	si p.goal_test(nc.state) alors
		renvoyer nc;
	sinon pour tout n de expand(nc, p.operators) faire
		rep <- recherche_prof_rec(n, p);
		si rep != Null alors
			renvoyer rep;
	fin si

	renvoyer Null;
}

//recherche avec collection
void recherche_prof_rec(Node nc, Problem p, List s){

	Node rep;

	si p.goal_test(nc.state) alors
		add(s, nc)
		renvoyer;
	sinon pour tout n de expand(nc, p.operators) faire
		recherche_prof_rec(n, p);
	fin si

	renvoyer;
}
```

8.
```
Bool test_etat_parent_identique(Node n){

	Node p = n.parent;

	tant que p != Null et p.state != n.state faire
		p <- p.parent;
	fin tant que

	renvoyer p != Null;
}

void recherche_prof_rec(Node nc, Problem p, List s){

	Node rep;

	si test_etat_parent_identique(nc) alors
		renvoyer Null;
	sinon si p.goal_test(nc.state) alors
		renvoyer nc;
	sinon pour tout n de expand(nc, p.operators) faire
		rep <- recherche_prof_rec(n, p);
		si rep != Null alors
			renvoyer rep;
	fin si

	renvoyer Null;
}
```

9. 
```
void recherche_prof_rec(Node nc, Problem p, List s){

	Node rep;

	si test_etat_parent_identique(nc) alors
		renvoyer Null;
	sinon si p.goal_test(nc.state) alors
		renvoyer nc;
	sinon pour tout n de expand(nc, p.operators) faire
		rep <- recherche_prof_rec(n, p);
		si rep != Null alors
			renvoyer rep;
	fin si

	delete nc;
	
	renvoyer Null;
}
```

10. Oui

- Si le problème a un nombre d'états fini et pas de circuit dans le graphe des états.

- Si on a une borne pour le nombre d'actions maximal d'une solution. 

- Recherche en profondeur iterative 

```
recherche_prof_iterative(Problem p){

	Borne b <- 0;
	Node n;

	Repeter
		n <- recherche_profondeur(p, b);
		b <- b + 1;
	jusqu'à n != Null;
}
```

# EXERCICE 2

1. M = Missionnaire, C = Cannibale, B = Barque, T = Traversé, G = Gauche, D = Droite

Règles :
- suffisamment de M et de C pour faire l'action
- pas de deséquilibre entre M et C sur les 2 rives (M = 0 ou C = 0 ou M = C)

Actions:
- T CC
- T MM
- T MC
- T C
- T M

Etats: (nb M à G, nb C à G, position B)
- But: (0, 0, D)
- Initial: (3, 3, G)

2. 

```
	 -------(3, 3, G)-------					(0, 0, D)--------------
	/ 			|			\						|		\		   \
(2, 2, D)	(3, 2, D)	(3, 1, D)					|	(0, 1, G)	(1, 1, G)	
	|												|
(3, 2, G)										(0, 2, G)---(0, 1, D)
	|															|
(3, 0, D)---(3, 1, G)---(1, 1, D)---(2, 2, G)---(0, 2, D)---(0, 3, G)
```

3.
```
	 -------(3, 3, G)-------
	/ 			|			\
(2, 2, D)	(3, 2, D)	(3, 1, D)
	|	  \
(3, 3, G)	(3, 2, G)
		  /		|	  \
(2, 2, D)	(3, 1, D)	(3, 0, D)
								\
								(3, 1, G)
```

# EXERCICE 3

6 variables pour decrire la maison :

X composé de 30 variables

Cigarette, Pet, 

X =  { Cx, Px, Nx, Sx, Dx, Lx } avec x <- {1..5}

chaque variable a un domaine de taille 5

D(c1) = D(C2) = D(C5) = { R, G, I, Y, B }
D(P1) = ...	  = D(P5) = { D, S, F, 4, 2 }

C2.1

C1 	N1
R 	E
G 	N
G 	S
G 	J
G 	U
I 	N
I 	S
I 	J
I 	U
 ...
Y	N
Y	S
Y 	J
Y	U
B 	N
B 	S
B 	J
B 	U
