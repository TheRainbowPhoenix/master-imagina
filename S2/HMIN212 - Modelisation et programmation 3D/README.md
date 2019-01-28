# HMIN212 - Modelisation et programmation 3D

## Sommaire [↺](#sommaire-)

- [Informations](#informations-)
	- [Examens](#examens-)
	- [Ressources](#ressources-)
- [Modelisation](#modelisation-)
	- [Vecteur 3D](#vecteur-3d-)
		- [Multiplication par un scalaire](#multiplication-par-un-scalaire-)
		- [Norme](#norme-)
		- [Produit scalaire](#produit-scalaire-)
		- [Produit vectoriel](#produit-vectoriel-)
		- [Projections](#projections-)
			- [Sur une droite](#sur-une-droite-)
			- [Sur un plan](#sur-un-plan-)
	- [Transformations](#transformations-)
		- [Translation](#translation-)
		- [Mise à l'echelle](#mise-à-l'echelle-)
		- [Rotation](#rotation-)
	- [Gestion des objets 3D](#gestion-des-objets-3d-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : Tout TP noté sur 2 (note final sur 22)

### Ressources [↺](#sommaire-)

- [Cours et TDs-TPs](https://moodle.umontpellier.fr/course/view.php?id=5880)

## Modelisation [↺](#sommaire-)

**Modele 3D** : on peut representer un modele 3D de plusieurs façon 

1. Ensemble de points
2. Ensemble de Surfaces
	- Discretes (Ensemble de point relié par des facettes)
	- Continue (Equation géométrique)
3. Ensemble de Volumes
	- Discret (Voxels = pixel 3D)
	- Continu (Equation géométrique)

### Vecteur 3D [↺](#sommaire-)

- **Vecteur** : un vecteur represente une direction dans l'espace

#### Multiplication par un scalaire [↺](#sommaire-)

Permet d'augmenter/diminuer la longueur/intensité d'un vecteur U

```
U'x = Ux * s
U'y = Uy * s
U'z = Uz * s
```

#### Norme [↺](#sommaire-)

La norme d'un vecteur U represente ça longueur/intensité elle se note |U|

```
|U| = sqrt(Ux² + Uy² + Uz²)
```

#### Produit scalaire [↺](#sommaire-)

Le produit scalaire de 2 vecteurs U, V est un multiplication de ces 2 vecteurs, il se note U.V

```
U.V = (Ux * Vx) + (Uy * Vy) + (Uz + Vz)
```

Il permet de determiné l'angle α existant entre 2 vecteurs

```
U.V = cos(α) * (|U| * |V|)
α = acos(U.V / (|U| * |V|)) 
```

- **Si U.V = 0** 
	- α = 90° ou 270° 
	- U et V sont orthogonaux

#### Produit vectoriel [↺](#sommaire-)

Le produit vectoriel entre 2 vecteurs U, V permet de calculer le vecteur orthogonal à U et V on le note U^V

```
      | (Uy * Vz) - (Uz * Vy) |
U^V = | (Uz * Vx) - (Ux * Vz) |
      | (Ux * Vy) - (Uy * Vx) |
```

Il permet egalement de determiné l'angle α existant entre 2 vecteurs

```
|U^V| = sin(α) * (|U| * |V|)
α = asin(U.V / (|U| * |V|)) 
```
- **Si U^V = 0** 
	- α = 0° ou 180°
	- U et V sont dits colinéaires

#### Projections [↺](#sommaire-)

##### Sur une droite [↺](#sommaire-)

```
|BA'| = BA.BC / |BC|

A'x = Bx + (Ux * |BA'|) 
A'y = By + (Uy * |BA'|) 
A'z = Bz + (Uz * |BA'|) 
```
##### Sur un plan [↺](#sommaire-)

**Projection orthogonale**

```
|MM'| = MA.n / |n|

M'x = Mx - (nx * |MM'|) 
M'y = My - (ny * |MM'|) 
M'z = Mz - (nz * |MM'|) 
```

**Projection non orthogonale**

```
|MI| = (|MM'| * |U| * |n|) / (U.n)
```

### Transformations [↺](#sommaire-)

#### Translation [↺](#sommaire-)

- Une translation represente le deplacement d'un objet en lui
	- On applique addition le vecteur de translation à chaque points de l'objet

#### Mise à l'echelle [↺](#sommaire-)

- On met à l'echelle un objet à partir d'un centre C et d'un rayon r
	- Pour chaque point on calcule le vecteur de mise à l'echelle entre le centre et ce point
	- Attention en fonction du centre choisie l'objet peut avoir sa forme modifié

#### Rotation [↺](#sommaire-)

- On applique un rotation selon un axe représenter par le vecter A et un angle α
	- On utillise des matrices de rotation pour chaques axes de A

```
                     | 1   0      0     |
Autour de l'axe OX = | 0 cos(α) -sin(α) |
                     | 0 sin(α) cos(α)  |

x' = x
y' = (y * cos(α)) - (z * sin(α))
z' = (y * sin(α)) + (z * cos(α))

                     | cos(α)  0 sin(α) |
Autour de l'axe OY = |   0     1   0    |
                     | -sin(α) 0 cos(α) |

x' = (x * cos(α)) - (z * sin(α))
y' = y
z' = (x * -sin(α)) + (z * cos(α))

                     | cos(α) -sin(α) 0 |
Autour de l'axe OZ = | sin(α) cos(α)  0 |
                     |   0      0     1 |

x' = (x * cos(α)) - (y * sin(α))
y' = (x * sin(α)) + (y * cos(α))
z' = z
```

**Matrice générale**

```
|       Ux² + (1 - Ux²) * cos(α)          Ux.Uy * (1 - cos(α)) - (Uz * sin(α))    Ux.Uz(1 - cos(α)) + (Uy * sin(α)) |
| Ux.Uy * (1 - cos(α)) + (Uz * sin(α))          Uy² + (1 - Uy²) * cos(α)          Uy.Uz(1 - cos(α)) - (Ux * sin(α)) |
| Ux.Uz * (1 - cos(α)) - (Uy * sin(α))    Uy.Uz * (1 - cos(α)) + (Ux * sin(α))         Uz² + (1 - Uz²) * cos(α)     |
```

### Gestion des objets 3D [↺](#sommaire-)

- Recuperation
	- Digitalisation
	- Lecture dans un fichier

- Habillage
	- Coloration
	- Materiaux (modifie les effets de la lumière)
		- Spéculaire
		- Diffuse
		- Ambiante
	- Texture

- Scene 3D
	- Eclairage (ce qui donne l'effet 3D)
	- Point de vue

- Animation
	- Rotation / Translation
	- Deformation

- OpenGl (glut)
	- GL_SMOOTH : lumière calculer par les points 
	- GL_FLAT: lumière calculer par surfaces 

### Courbes paramétriques

Peut être vue comme le deplacement d'un point dans l'espace

defini par
	- un scalaire u ∈ [0, 1]
	- l'abscisse curviligne s ∈ [0, l]
	- valeur de la courbure k = 1 / r

```
            | x(u) = fx(u) |
u -> p(u) = | y(u) = fy(u) |
            | z(u) = fz(u) |
```

#### Courbes Cubiques

de la forme P(u) = au³ + bu² + cu + d

- defini par 
	- point de depart P0(u=0)
	- point d'arrivé P1(u=1)
```

P(u) 

```

#### Courbes de bézier

Un point de la courbe est une combinaison affine des points de contrôle Pi, si u ∈ [a, b]

```
equation
```

#### Courbes B-Splines

- ensemble de point de contrôle
- vecteur de noeud
- ensemble de multiplicité
- ensemble de poids

### Surfaces paramétriques