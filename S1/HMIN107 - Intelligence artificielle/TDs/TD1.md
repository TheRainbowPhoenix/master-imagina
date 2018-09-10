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

(recherche en largeur améliorée)

13 noeuds générés, 11 explorés.

6. (recherche en profondeur améliorée)

11 noeuds générés, 9 explorés.

7.



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

