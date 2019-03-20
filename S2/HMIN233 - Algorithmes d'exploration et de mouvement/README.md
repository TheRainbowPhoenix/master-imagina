# HMIN233 - Algorithmes d'exploration et de mouvement

## Sommaire [↺](#sommaire-)

- [Informations](#informations-)
	- [Examens](#examens-)
	- [Ressources](#ressources-)
- [Mouvement](#mouvement-)
	- [Flocking](#flocking-)
	- [Flocking en V](#flocking-en-v-)
	- [Flocking vectoriel](#flocking-vectoriel-)
		- [Vecteur en Netlogo](#vecteur-en-netlogo-)
	- [Evitement](#evitement-)
- [Exploration](#exploration-)

## Informations [↺](#sommaire-)

- Anciennement : HMIN213

### Examens [↺](#sommaire-)

- Note final : Exam seulement sur la partie mouvement

### Ressources [↺](#sommaire-)

- [Cours et TDs-TPs](http://www.lirmm.fr/~ferber/GMIN20A-exploration/)

## Mouvement [↺](#sommaire-)

### Flocking [↺](#sommaire-)

- **flock** : designe un groupe d'agents en anglais (Uttilisé pour les groupe d'oiseaux, mammifères, etc...)
- **flocking** : Mouvement d'escadrille, les agents ce coordinnent entre eux avant de se deplacer

Regle general du flocking:

1. Répulsion (separate) : Reste à distance des agents voisins (inverse de la moyenne des direction des plus proche)
2. Alignement (align) : S'adapte à la vitesse et direction des agents voisins (somme des direction/vitesses)
3. Centrage (cohere) : Se centre par rapport aux agents voisins (moyenne des directions vers les agents voisins)

### Flocking en V [↺](#sommaire-)

1. (cohere) : si l'agent et trop loin des autres voisins il accelère pour les rattraper
2. si l'agent et assez près d'un autre il se met sur l'un de ses cotés (pour ainsi avoir une meilleure vue)
3. (separate) : si l'agent et trop proche d'un autre il ralentit
4. (align) : si les autres conditions sont remplies la vitesse/direction s'adaptent aux voisins

### Flocking vectoriel [↺](#sommaire-)

On compose l'ensemble des motivations et décisions d'actions sous la forme de vecteurs

- Va = Alignement
- Ve = Répulsion
- Vc = Centrage
- Vd = Direction resultante

Et on somme ces vecteurs en fonction de leurs importances

**Vd = a.Va + e.Ve + c.Vc**

Avec a, e, f les coefficients de forces des vecteurs (determinant leurs importance).

#### Vecteur en Netlogo [↺](#sommaire-)

```netlogo
;; vect est une liste de 2 valeurs (list x y)

to-report angle-from-vect [vect]
  let a atan item 0  vect item 1 vect
  report a
end

;; Netlogo n'est pas dans le repère classique: le 0 est vers le haut
;; et les angles tournent vers la droite. De ce fait, x = sin angle dans ce référentiel...

to-report vect-from-angle [angle len]
  let l (list (len * sin angle) (len * cos angle))
  report l
end

to-report multiply-scalar-vect [factor vect]
  report (list (item 0 vect * factor) (item 1 vect * factor))
end

to-report add-vect [v1 v2]
  report (list (item 0 v1 + item 0 v2) (item 1 v1 + item 1 v2))
end

to-report vect-length [vect]
  let x item 0 vect
  let y item 1 vect
  report sqrt (x * x) + (y * y)
end
```

**Exemple** : ![flocking-vectioriel.nlogo](TDs-TPs/TP1%20-%20Flocking%20et%20evitement%20d'obstacles/flocking-vectoriel.nlogo)

### Evitement [↺](#sommaire-)

#### Evitement vectoriel [↺](#sommaire-)

plus simple quand obstacle : on retourne en arrière avec un peu d'aléatoire puis on recommence

```netlogo
to avoid-obstacles
  set obstacles patches
  in-cone vision angle-avoidance with [pcolor != black]
  if any? obstacles [
    flee
  ]
end

to flee
  let obstacles-in-front obstacles in-cone 3 angle
  if any? obstacles-in-front [
    rt 180
    rt random 10
    lt random 10 
  ]
end
```
#### Champ de potentiel [↺](#sommaire-)

champ de potientiel utilliser pour representer les repulstion et l'attraction

## Exploration [↺](#sommaire-)

### Heuristique

- Explorer tout l'espace et essayer de trouver le meilleur chemin à partir de ce que l'on a exploré.
  - Algorithme de Dijkstra

- Essayer d'aller vers le but en utillisant une heureustique (se rapprocher du but)
  - Algorithme de best-first

- Essayer de combiner le mieux dans ces deux approches
  - Algorithme de A*

(examen : ecrire les differents algorithmes et dire les différences)

- On prend un noeud et on génere les noeuds voisins

- terminaison : soit trouvé but soit liste des in-nodes vides

- merge nodes fait beaucoup : 

quand on genere la liste des voisins on réorganise les noeuds avec merge nodes
  - si cout inferieur


modification du A* possible en modifiant la puissance de la fonction de (cout de chemin)

- Ajouter des poids sur le A*

## Examen

- meme choses
- 2 parties
theorie/cours
pas de docs
5 séances

partie cours
dessiner graphe exploration quels sont es noeuds executée
qu'est ce que ça change quand on change les paramètres
dessiner vecteur
savoir ce qu'il se passe
partie code netlogo

# Algorithmes d'exploration

## Examen

- Voisinage et calcul du nombre

## Question 1

Le voisinnage doit pouvoir atteindre n'importe quelle solutions avec un infinité de temps.

- **Voisinage 1**  : Echanger 2 voitures
	- Nb voisins = n * (n - 1) / 2 
- **Voisinage 1'** : Echanger 2 voitures en conflit
	- Nb voisins = n - 1
- **Voisinage 2**  : Couper en 2 et echanger les 2 sous-séquences
	- Nb voisins = n 
- **Voisinage 3**  : Echanger 2 voitures à la suite
	- Nb voisins = n - 1 
- **Voisinage 4**  : Renverser 1 sous-séquence de longueur k
	- Nb voisins = n - k + 1

## Question 2

A B C B A C B

viol : B C B, A C
cout : 2

## Question 3

On atteind un minimum local.

## Question 4

**Marche en 7 :**

A B C B A C B
B A C B A C B
B A B C A C B
B A B C A B C
B A C B A B C
B C A B A B C
C B A B A B C

**Marche en 4 :**

A B C B A B C
B A C B A B C
B C A B A B C
C B A B A B C
