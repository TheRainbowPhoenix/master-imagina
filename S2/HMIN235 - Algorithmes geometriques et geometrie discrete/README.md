# HMIN235 - Algorithmes geometriques et geometrie discrete

## Sommaire [↺](#sommaire-)

- [Informations](#informations-)
  - [Examens](#examens-)
  - [Ressources](#ressources-)

## Informations [↺](#sommaire-)

- Anciennemnt : HMIN214

### Examens [↺](#sommaire-)

- Note final : max(50% CC + 50% Exam, Exam)
	- Examen
	- Controle
		- participation
		- rendu de l'exercice

### Ressources [↺](#sommaire-)

- [Cours et TDs-TPs 1](http://www.lirmm.fr/~mountaz/Ens/AlgoGeo/)
- [Cours et TDs-TPs 2](https://www.lirmm.fr/~bessy/AlgoGeo/accueil.html)

## Algorithmes géométriques

**Géométrie** : science de touts les types d'espaces

### Applications linéaires

s'applique aux éléments d'un espaces vectoriel

**Endomorphisme** : l'espace de départ et le même que celui d'arrivé

- Homothétie (de rapport k ∈ R), de centre O
- Rotation, d'angle α, de centre O
- Symétrie par rapport à une droite, passant par O
- Homothétie (de rapport k ∈ R), de centre O
- Homothétie (de rapport k ∈ R), de centre O

toute application linéaire à une représenation par une matrice

# TD-TP-4 Exercice 2

- image 2 : on utilise un simple rotation
- image 3 :

### Applications affines

### Transformations bifocales

### Transformations fisheye

## Géometrie discrete

## Trioker

- Point2 : (double x, y)
	- translate(Vector2)
	- rotate(Point2, double angle)

- Vector2 : (double x, y)
	- add(Vector2)
	- soustract(Vector2)
	- multiply(double)
	- divide(double)
	- equals(Vector2)
	- normalize
	- double angle()
	- double lengthSquared()
	- double length()
	- double distanceSquared()
	- double distance()

- Triangle : (3 Point2)
	- center()
	- contains(Point2)
	- translate(Vecteur2)
	- rotate(double)

- Trioker : (Triangle && String label)
	- rotateLeft()
	- rotateRight()
	- invert()
