# HMIN212 - Modelisation et programmation 3D

## Sommaire [↺](#sommaire-)

- [Informations](#informations-)
  - [Examens](#examens-)
  - [Ressources](#ressources-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : Tout TP noté sur 2 (note final sur 22)

### Ressources [↺](#sommaire-)

- [Cours et TDs-TPs]()

**Modele 3D** :
	- Ensemble de points
	- Surfaces
		- Discretes (Ensemble de point relié par des facettes)
		- Continue (Equation géométrique)
	- Volumes
		- Discret (Voxels = pixel 3D)
		- Continu (Equation géométrique)

# Vecteurs

**Vecteur** :
	- Direction dans l'espace entre 2 points
	- Permet de translater

norme |U| = sqrt(U_x² + U_y² + U_z²)

produit scalaire U.V = U_x * V_x + U_y * V_y + U_z + V_z
U.V = cos(α) * |U| * |V|

si U.V = 0 -> α = 90° ou 270° 
produit vectoriel :
donne un vecteur orthogonal au deux autres
|U^V| = sin(α) * |U|.|V|

si U^V = 0 :
	- α = 0° ou 180°
	- U et V sont dits colinéaires

# Matrices

**Matrices** : un ensembles de vecteurs

# Projections

un point sur une droite :
|B->A'| = B->A.B->C / |B->C|

un point sur un plan :

# Transformation

**deplacement**

**mise à l'echelle** : probleme de choisir un bon centre

**rotation** : 
	on utillise des matrices de rotation pour chaques axes

# Gestion des objets 3D

Step/Iges : format ouverts

Creation d'objet

Recuperation
	- Digitalisation
	- lecture dans un fichier

Habillage
	- coloration
	- eclairage (ce qui donne l'effet 3D)
	- materiaux (modifie les effets de la lumière)
		- spéculaire
		- diffuse
		- ambiante
	- texture

Scene 3D
	- eclairage
	- point de vue

Animation
	- rotation / translation
	- deformation


GL_SMOOTH : lumière calculer par les points 
GL_FLAT: lumière calculer par surfaces 

# Courbes paramétriques 

# Surfaces paramétriques