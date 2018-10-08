# TD2

# Exercice 1

**Question 1:**
```
                    {}
                    /
WA                 R
                  /
Q                R
                / \
T              R   G
                  /|\
SA               R G B
                    /
NSW                R
                  /|\
                 R G B
```
**Question 2:**

(voir feuille)

**Question 3:**

| Etape | MAX_CARD           | MAX_DEG  | LEX |
|-------|--------------------|----------|-----|
| 1     | WA,NT,R,NSW,V,SA,T | SA       |     |
| 2     | WA,NT,Q,NSW,V      | NT,Q,NSW | NSW |
| 3     | Q,V                | Q        |     |
| 4     | V,NT               | NT       |     |
| 5     | WA,V               | NA,V     | V   |
| 6     | WA                 |          |     |
| 7     | T                  |          |     |

**Question 4:**
```
                    {}
                    /
SA                 R
                  / \
NSW              R   G
                    /|\
Q                  R G B
                      / \
NT                   R   G
                        /|\
V                      R G B
                          /|\
WA                       R G B
                            /
T                          R 
```

**Question 5:**

**Question 6:**

**Question 7:**
```
Boolean viol(Assignation A)
	si portée inclus dans dom(A) alors
		si A[portée] n'appartient pas à tuples alors
			retourner vrai
		finsi
	finsi
	retourner faux
fin

Boolean consistant(Variable x)
	Pour toute c appartenant à contraintes contenant x faire
		si c.viol(A) alors
			retourner false
		finsi
	finpour
	retourner true
fin
```

# Exercice 3

```
X = {R1, R2, R3, R4}

pour tout x appartenant à X

D(r) = 1..16 ou D(r) = {}
```