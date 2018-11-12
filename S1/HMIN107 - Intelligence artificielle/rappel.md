# Rappel Logique des propositions

## Syntaxe

- Symboles propositionnels (variables propositionnels) A, B, C...
- Connecteurs : !, ^, v, ->, <->, ...
- Formules (fbf) : A -> B ^ !c

(1) A, B, C... atomes

(2) si A et B sont des fbf, !A, (A^B), (AvB), (A->B) sont des fbf

**literal** : symbole ou negation d'un symbole.
**clause** : disjonction de litteraux (Ex. A v !B v C v !D)
**CNF** : forme normale conjonctive (conjonctions de clauses)

## Sémantique

- Vocabulaire E
- Interpretation I:E -> {vrai, faux} 

v(A -> B, I) = faux ssi A et B faux

I **modèle** de F = v(F, I) = vrai
F **satisfiable** s'il existe un modèle de F
F_1 et F_2 sont **équivalente** si ∀I, v(F_1, I), = v(F_2, I)

Toute formule est equivalente à 1 CNF

## Conséquence logique

De H_1, ...., H_k, peut-on conclure C ?

{H_1, ..., H_k} |= C
H_1, ..., H_k |= C
H_1 ^ ... ^ H_k |= C

Tout modèle I de H_1 ^ ... ^ H_k est un modèle de C

**Théoreme fondamental** : {H_1, ..., H_k} |= C ssi H_1 ^ ... ^ H_k ^ !C est insatisfiable

F_1 !(|=) F2 != F_1 |= !(F2)