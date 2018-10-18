# EXERCICE 1

**Question 1**
```
Un ensemble d'au plus 3 sequences. 
chaque séquence est ordonnée du plus haut au plus bas.

- Etat initial : { <C, A>, <B>, <> }
- Etat but     : { <B, A, C>, <>, <> }
- Action :
	- Ne rien faire, 
	- Deplacer un cube de haut de sequence en haut d'une autre sequence
```

**Question 2**

(espace de recherche)

**Question 3**

(parcours en largeur)

```
93 noeuds générés, 31 explorés.
```

**Question 4**
```
Avec le parcour en profondeur on ré-explore l'etat initial à l'infini.
```
**Question 5**
```
Pour éviter de ré-etudier un état, au moment de la fonction expand on peut vérifier si l'etat appartient déjà à une table d'états deja générés.
```

(recherche en largeur améliorée)

```
13 noeuds générés, 11 explorés.
```
**Question 6**

(recherche en profondeur améliorée)

```
11 noeuds générés, 9 explorés.
```

**Question 7**
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

**Question 8**
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

**Question 9** 
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

**Question 10**
```
Oui

- Si le problème a un nombre d'états fini et pas de circuit dans le graphe des états.
- Si on a une borne pour le nombre d'actions maximal d'une solution. 
- Recherche en profondeur iterative 

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

**Question 1**
```
M = Missionnaire, C = Cannibale, B = Barque, T = Traversé, G = Gauche, D = Droite

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
```


**Question 2**
```
	 -------(3, 3, G)-------					(0, 0, D)--------------
	/ 			|			\						|		\		   \
(2, 2, D)	(3, 2, D)	(3, 1, D)					|	(0, 1, G)	(1, 1, G)	
	|												|
(3, 2, G)										(0, 2, G)---(0, 1, D)
	|															|
(3, 0, D)---(3, 1, G)---(1, 1, D)---(2, 2, G)---(0, 2, D)---(0, 3, G)
```

**Question 3**
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
```
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

Pour tout i appartenant à 1..5 :
D(Ni) = {Sp, Eng, Uk, Ja, No}
D(Ci) = {Iv, Red, Blue, Green, Yellow}

C2 : 
<N1, C1> : {(Eng, Red)}U({Sp,Ja,Vh,No}x{Iv,G,Bl,Ye})
  ...
<N5,C5>

5 Contraintes binaires de 17 tuples

C3 :
<P1, N1> : {(Sp, Dog)}U({Fra,Uk,Ja,N$}x{Sn,Ze,Fo,H$})
...
<P5,N5>

C4 : 5 contraintes binaire de 17 tuples
C5 : ----------------------------------
C6 : <C1,L1,C2,L2> : ({Iv, Bl, Ye, Gr, Red}x1..5x{1,2,3,4,5}x1..5)\I

I (Interdits) = {
	(G,1,I,2),(G,1,I,3),
	(G,1,I,4),(G,1,I,5),
	(G,2,I,3),(G,2,I,4),
	(G,2,I,5),(G,3,I,1),
	(G,3,I,4),(G,3,I,3),
	(G,4,I,1),(G,4,I,2),
	(G,4,I,5),(G,5,I,1),
	(G,5,I,2),(G,5,I,3)
}

<D1,L1> : {(Milk,3)}U({Wat,Orange,Coffe,Tea}x{1,2,4,5})
...
<D5,L5>

C6 : 
<C1,C2> : (CxC)\{(G2,Iv)}\({Iv}x{B,Y,R})\({B,Y,R}x{G})\({G2,Iv})
<C2,C3> : 
<C3,C4> : 
<C4,C5> :
```

**Inversion de modelisation:** 
```
X = {
	Ye,Bl,Re,Gr,Iv,
	Sp,En,Ja,Uk,No,
	...
} <- 25 variables

Pour tout x appartenant à X, D(x) = 1..5

C1  _ Domaine
C2  _ <Eng,Red> : Eng = Red : {(1,1),(2,2),(3,3),(4,4),(5,5)}
C3  _ Spa = Dog       
C4  _ Cof = Green     
C5  _ Uk = Tea        
C6  _ <Green,Ivory> : Green = Ivory+1 {(2,1),(3,2),(4,3),(5,4)}  
C7  _ Og = Sn
C8  _ Koal = Yel
C9  _ <Milk> : Milk = 3
C10 _ <Nor> : Nor = 3
C11 _ <Chest,Fox> : chest = fox-1 ou chest = fox+1
C12 _ Même chose
C13 _ LS = Orange
C14 _ Jap = Parliaments
C15 _ <Blue> : {2}

C6 : 
|G|I|
|-|-|
|2|1|
|3|2|
|4|3|
|5|4|

C12 : 

|Yel|Horse|
|---|-----|
|2  |1    |
|3  |2    |
|4  |3    |
|5  |4    |
|1  |2    |
|2  |3    |
|3  |4    |
|4  |5    |

C11 : 

|Chest|Fox|
|-----|---|
|2    |1  |
|1    |2  |
|3    |2  |
|2    |3  |
|4    |3  |
|3    |4  |
|4    |5  |
|5    |4  |

D(Red)    = {1,3,4,5}
D(Yellow) = {1,3,4,5}
D(Green)  = {1,3,4,5}
D(Ivory)  = {1,3,4,5}
D(Jap)    = {2,3,4,5}
D(Uk)     = {2,4,5}
D(Dog)    = {2,3,4,5}
D(Water)  = {1,2,4,5}
D(Orange) = {1,2,4,5}

AllDiff(Yell,Iv,Green,Red)

AllDiff(Jap,Uk,Dog,Red)

AllDiff(Horse,Dog,Zeb,Fox,OldGold)

AllDiff(Water,Orange,Uk,Gi)

AllDiff(OldGold,Chest,Jap,Yell)
```