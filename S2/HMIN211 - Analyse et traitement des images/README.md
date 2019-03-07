 HMIN211 - Analyse et traitement des images

## Sommaire [↺](#sommaire-)

<!-- MarkdownTOC autolink="true" -->

- [Informations ↺](#informations-%E2%86%BA)
	- [Examens ↺](#examens-%E2%86%BA)
	- [Ressources ↺](#ressources-%E2%86%BA)
- [Analyse d'une image ↺](#analyse-dune-image-%E2%86%BA)
	- [Representation mathématiques des images](#representation-math%C3%A9matiques-des-images)
	- [Distance entre deux pixels](#distance-entre-deux-pixels)
	- [Connexité](#connexit%C3%A9)
	- [Histrogramme](#histrogramme)
- [Traitement ↺](#traitement-%E2%86%BA)
	- [Explication au tableau pour TP3](#explication-au-tableau-pour-tp3)
	- [Suite du cours en pdf](#suite-du-cours-en-pdf)
	- [Système visuel humain](#syst%C3%A8me-visuel-humain)
	- [Erosion, dilatation, ouverture, fermeture](#erosion-dilatation-ouverture-fermeture)
	- [Filtrage d'images](#filtrage-dimages)
		- [Pretraitement d'images](#pretraitement-dimages)
			- [Supprimer le bruit](#supprimer-le-bruit)
		- [Filtre moyenneur](#filtre-moyenneur)
		- [Filtre gaussien](#filtre-gaussien)
		- [Filtre exponentiel](#filtre-exponentiel)
		- [Filtre gaussien 3x3](#filtre-gaussien-3x3)
		- [Filtre non linaires](#filtre-non-linaires)
		- [Filtre median](#filtre-median)
	- [Colorimetrie](#colorimetrie)
	- [Filtres passe haut \(Détection de contours\)](#filtres-passe-haut-d%C3%A9tection-de-contours)
		- [1) Interpretation](#1-interpretation)
		- [2) Gradiant d'une image](#2-gradiant-dune-image)
		- [3) Utilisation du gradient](#3-utilisation-du-gradient)
- [Opérateurs dérivatifs du second ordre](#op%C3%A9rateurs-d%C3%A9rivatifs-du-second-ordre)
	- [a) Laplacien sous forme discrète](#a-laplacien-sous-forme-discr%C3%A8te)
	- [b) Filtres dérivatifs \(1er ordre, 2eme ordre\)](#b-filtres-d%C3%A9rivatifs-1er-ordre-2eme-ordre)
		- [1) Filtre de dérivation double passe \(Sobel, hewit\)](#1-filtre-de-d%C3%A9rivation-double-passe-sobel-hewit)
		- [2) Approximation du gradient](#2-approximation-du-gradient)
		- [3) Filtre de renforcement](#3-filtre-de-renforcement)
		- [4) Filtre pass haut](#4-filtre-pass-haut)
	- [Projet](#projet)

<!-- /MarkdownTOC -->

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- **Note final** : 60% Exam + 40% TP
	- Controle continu
		- Compte rendu de TP (pdf avec resultat des TPs) (CR_TP1_NOM.pdf)
		- TP à deux
	- Examen 
		- Question sur un article à lire
		- Question de calcul de distance selon repère (0, 0) en haut à gauche
		- Nouveauté : Même type d'exam avec en plus feuille mémo recto verso + question sur les presentations des autres

### Ressources [↺](#sommaire-)

- [Cours et TDs-TPs](http://www.lirmm.fr/~wpuech/enseignement/master_informatique/Analyse_Traitement_Image/)

## Analyse d'une image [↺](#sommaire-)

- 2 branches différentes en images
	- Analyse et traitement : à partir de capteurs on recupère les données de l'image
	- Informatique graphique : à partir de modèles et synthèses on créée l'image

**Image spatiotemporel** : capteur recupère des informations qui varie en fonction du temps (images sismiques, )

- **Etape en images**
	- **Traitement/Prétraitement**
		- Compression : réduction du volume de l'image
		- Restauration : correction des défauts dus à une source de dégradation
		- Amélioration : modification de l'image dans le but de la rendre plus agréable à l'oeil
	- **Analyse**
		- Approche de segmentation
			- Ressemblance : segmentation par région (en regardant les pixel d'une zone qui se ressemblent)
			- Dissemblance : segmentation par contours (en regardant les fortes variation d'une images avec des dérivé)
	- **Interprétation** : passage de la description structurelle à la description sémantique en regard à certains objectifs

- Les dix commandements de la vision par ordinateur dans le domaine industrielle
	1. on respecte les lois de la physique
	2. on s'inquite de ce qu'on mesure avant de calculer
	3. on utilise des grandeurs reproductibles
	4. on se soucie de la qualité des données plutot que la vitesse des calculs
	5. l'algorithme et plus important que le code
	6. on ce soucie de l'aval et de l'amont de la reconnaissance des images
	7. on effectue les essais sur plusieurs pièces sans tricher sur l'eclairage
	8. on mes des chiffres sur les defauts (seuil de tolérances)
	9. la simplicité est un idéal 
	10. on ne doit pas seulement utiliser des caméra vidéos (ils existent d'autres capteur)

### Representation mathématiques des images

**Image** : discrétisation d'un phénomène continu.
	- Image binaire : (p, M) = (1, 1)
	- Image en niveaux de gris : p = 1 et M = 255
	- Image en couleur : p = 3 et M = 255

Avec p le nombre de dimension et M le nombre max d'intensité l'umineuse.

**Pixel** : "Picture element" en anglais 
	- Position dans une image
	- Intensité lumineuse

**Maillage** : arrangement géométrique des pixels dans l'image
	- Maillage carré : pixel rectangulaire capteur CCD arrangement des pixels les uns par rapport aux autres
	- Maillage hexagonal : pixel hexagonaux chaque pixels on 6 voisins avec un coté commun (permet un meilleure calcul des distances comme chaque voinsins sont à equidistances)
	- Maillage triangulaire : pixel en triangles

### Distance entre deux pixels

distance entre deux pixels P(xp, yp) et Q(xq, yq)

- **Distance de Manathan** : `d(P, Q) = |xp - xq| + |yp - yq|` (nombre de pixel qu'on utilise pour faire un chemin a chaque point)
- **Distance euclidienne** : `d(P, Q) = sqrt((xp - xq)² + (yp - yq)²)`
- **Distance échiquier**   : `d(P, Q) = max(|xp - xq|, |yp - yq|)`

### Connexité

- Ordres de connexité dans une image
	- 2D
		- Connexité 4 (degré 1) : pixel à 4 voisins (distance de manhantan)
		- Connexité 8 (degré 2) : pixel à 8 voisins (distance de l'échiquier)
	- 3D 
		- Connexité 6 (Degré 1) : voxel qui ont une **face** commune
		- Connexité 12 + 6 (Degré 2) : voxel qui ont une **arête** commune
		- Connexité 8 + 12 + 6 (Degré 3) : voxel qui ont un **sommet**

### Histrogramme

l'histogramme H de l'image I calcul l'occurence des pixels dont l'intensité est x

`H(x) = Card{P : I(P) = x}`

## Traitement [↺](#sommaire-)

- **Histrogramme** : à partir d'une image on calcul les occurences des niveaux de gris.
	- Permet de trouver les grands pics (Qui ont une forte chance de representer des objet de l'image)
	- Permet de faire de la **segmentation**
	- Permet de faire de la **recherche/indexation**
	- Permet de faire de la **normalisation** (ramener une image sur une autre echelle, une autre norme)

### Explication au tableau pour TP3

a) Modification d'histogramme

Elle va nous permettre d'ameliorer le contenu d'une image à l'aide d'une transformation sur chaque pixels (transformation ponctuelle de l'intensité)

Image I -> pixel p(i), i ∈ [0, N-1]

p(i) -> p'(i)

- ne doit pas modifier les régions
- doit respecter la relation d'ordre

p(i) < p(t) : p'(i) <= p'(j)



(dessin histogramme random avec un amin à 1/4 et un amax à 3/4)

avant amin aucune valeur n'atteint, après amax, aucune valeur n'atteint amax ce qui veut dire qu'on peut remettre à l'echelle l'histogramme

avec amin à 0 et amax à 255

\(p'(i) = T(p(i)) = α + βp(i)\)
\(p(i) = a\_{min} → p'(i) = 0\)
\(p(i) = a\_{max} → p'(i) = 255\)

si on veut rester sur une extension dynamique, il nous faudra supprimer les bruits (pixel avant et après amin) en les mettant à amin amax cepandant cela à pour effet de creer un pic d'intensité sur les valeurs limites

a_1) extensioni dynamique d'histogramme

Objectif : rendre le plus plat possible un histogramme. et renforcer les contrastes sur les détails de l'image.

recherche et indexation :

Image de référene Ir

Image I → spécification par rapport à Ir

a_2) egalisation d'histogramme

a_3) spécialisation d'histogramme

### Suite du cours en pdf

### Système visuel humain

- Composants de l'oeil
	- **Cornée** : filtre
	- **Iris** : diaphragme (varie en fonciton de l'intensité lumineuse)
	- **Cristalin** : optique + focus (lentille à focal variable)
	- **Rétine** : couche photo-sensible (120 millions de récepteurs : cones et battonnets)
	- **Macula** : zone centrale qui contient la plus grande quantité de cellule photo-sensible
	- **Nerf optique** : transporte et traite l'information pour les neuronnes

**Cônes** : peu sensible (6 à 7 millions de 3 types : RVB) detecte les couleurs, utiles dans la journées.
**Bâtonnets** : tres sensible (120 millions) utile de nuits, saturé dans la journées.



### Erosion, dilatation, ouverture, fermeture

fond de l'image en blanc et objet en noir

**erosion** : si j'ai blanc voinsin blanc

**dilatation** : si j'ai objet tous les voisin deviennent noir

- **ouverture** (ED) : erosion en premier puis dilatation sur le resultat
	- supprime les point d'objet isolé 

- **fermeture** (DE): dilatation en premier puis erosion sur le resultat
	- supprime les point du fond isolé dans l'objet

### Filtrage d'images

#### Pretraitement d'images
 
##### Supprimer le bruit

#### Filtre moyenneur

\(h(x, y) = {1 \over t^2}\)

#### Filtre gaussien

\(h(x, y) = { {1 \over 2πθ^2} e^{-{x^2 + y^2 \over 2θ^2}} }\)

avec θ taille de masque h.

#### Filtre exponentiel

\(h(x, y) = {{β \over 4} e^{-β(|x|+|y|)}}\)

**Remarque** : 3 filtres doux
- preserve les couleurs moyennes
- supprime les hautes fréquences

**Filtre passe bas** : FPB supprime les bruits (basse fréquences)

#### Filtre gaussien 3x3

\(h(m, n) = { {1 \over 16} \begin{bmatrix} 1 & 2 & 1\\ 2 & 4 & 2\\ 1 & 2 & 1 \end{bmatrix} }\)

#### Filtre non linaires

- filtre homomorphiques
- filtre adaptatif
- filtre morphologique

#### Filtre median

Trier les valeurs des pixels voinsins à p(i, j)

\(p_1(i,j) <= p_2(i, j) <= ... <= p_n(i, j)\)

\(h(m, n) = { {1 si p_5(i,j)} 0 sinon }\)  

### Colorimetrie

**Trichromie** : couleur perçue par un humain → 3 couleurs de base avec un spectre éloigné
**Trivariance** : couleur Cx fonction de λ et des luminances de Cλ et de Cb
**Synthèse soustractive** : en partant du blanc, on soustrait de couleurs jusqu'à atteindre le noir.
**Synthèse additive** : inverse de la synthèse soustractive.

Y = 0.3 * R + 0.6 * G + 0.1 * B
Luminance Y = 0.299 * R + 0.587 * G + 0.114 * B

**RGB to YUV**

Y = ...
U = 0.492 (B - Y) + 128
V = 0.877 (R - Y) + 128

**YUV to RGB**

R = ((Cr - 128) / 0.877) + Y / 0.587
G = (Y - O.299 * R - 0.114 * B)
B = ((Cb - 128) / 0.877) + Y

**RGB to YCrCb**

Y = ...
Cb = (B - Y) / (2 - 2 * 0.114 * B) + 128		(Cb = a * (B - Y))
Cb = (R - Y) / (2 - 2 * 0.229 * R) + 128		(Cr = b * (R - Y))

**RGB to YCrCb**

Y = 0.299 * R + 0.587 * G + 0.114 * B
...

### Filtres passe haut (Détection de contours)

#### 1) Interpretation

Detection de formes avec les profils de ligne

- Aucune variation
- marche
- rampe
- pic
- toit

Example avec une courbe rampe, en uttilisant
- une dérivé on aura une valeurs max sur le points d'inflexion
- une dérivé seconde on cherche le passage par zero

#### 2) Gradiant d'une image

Image : I(x, y) → gradiant vertical


\(I_x(x, y) = { dI(x, y) \over dx }\)

\(I_y(x, y) = { dI(x, y) \over dy }\)

**Module du gradiant**

\(G = \sqrt{ I_x^2 + I_y^2 } = max(I_x, I_y)\)

**Phase du gradiant**

\(ϕ = \arctan{I_y \over I_x}\)

**Discretisation**

\({ dI(x, y) \over dx } = { δI[i, j] \over δj } = I_j[i, j] = I[i, j+1] - I[i, j]\)

\({ dI(x, y) \over dy } = I_i[i, j] = I[i+1, j] - I[i, j]\)

\(G = \sqrt{I_i[i, j]^2 + I_j[i, j]^2}\)

#### 3) Utilisation du gradient

- Filtrage de l'image
- Calcul l'image du gradient
- Recherche des maximums locaux dans l'image du gradiant dans la direction de la phase
- Seuillage par hystéresis (analogie chauffage)

```
si p(i,j) > T_h
	alors maximum local
si p(i,j) < T_b
	alors non retenu
2eme passe

si T_b < p(i,j) < T_h et au moins N(p(i,j))
	alors p(i,j) Maximum local
```

| Algo \ Verité   | Positif        | Négatif       |
|-----------------|----------------|---------------|
| Positif         | True positif   | False positif |
| Negatif         | False negative | True negative |

filter({1/9, 1/9, 1/9,
		1/9, 1/9, 1/9,
		1/9, 1/9, 1/9});

## Opérateurs dérivatifs du second ordre

dérivé premiere → valeur maximale
dérivé seconde → valeur du point d'inflexion

### a) Laplacien sous forme discrète

\(\begin{bmatrix} 0 & -1 & 0\\ -1 & 4 & -1\\ 0 & -1 & 0 \end{bmatrix}\)

**Utilisation** :

- Filtrage de l'image : FPB
- Pour chaque point : gradient → Norme, Phase
- Calcul la carte du laplacien
- Carte du laplacien : 
	- Rechercher les passages par zéro dans la direction de la phase du gradient
	- Affectation de la norme du gradiant au passage par zéro
- Seuillage par hysérésis

### b) Filtres dérivatifs (1er ordre, 2eme ordre)

#### 1) Filtre de dérivation double passe (Sobel, hewit)

\(I_j[i,j] = h_j[i,j] * I[i,j]\)

\(I_i[i,j] = h_i[i,j] * I[i,j]\)

\(h_j = { \begin{bmatrix} 1 & 0 & -1\\ C & 0 & -C\\ 1 & 0 & -1 \end{bmatrix} }\)

\(h_i = { \begin{bmatrix} 1 & C & 1\\ 0 & 0 & 0\\ -1 & -C & -1 \end{bmatrix} }\)

si C = 1 filtre de sobel, si c = 2 filtre de hewit

#### 2) Approximation du gradient

\(\begin{bmatrix} -1 & 1 \end{bmatrix}\) \(\begin{bmatrix} -1 & 2 & -1 \end{bmatrix}\) \(\begin{bmatrix} -1 & 0 & 1 \end{bmatrix}\)

\(\begin{bmatrix} -1 \\ 1 \end{bmatrix}\) \(\begin{bmatrix} -1 \\ 2 \\ -1 \end{bmatrix}\) \(\begin{bmatrix} -1 \\ 0 \\ 1 \end{bmatrix}\)

\(\begin{bmatrix} 0 & 1 & 0\\ 1 & 0 & -1\\ 0 & -1 & 0 \end{bmatrix}\)

#### 3) Filtre de renforcement

FPB : ∑ coefs = 1 

\(\begin{bmatrix} 0 & -1 & 0\\ -1 & 5 & -1\\ 0 & -1 & 0 \end{bmatrix}\)

#### 4) Filtre pass haut

FPH : ∑ coefs = 0 

\(\begin{bmatrix} -1 & -1 & -1\\ -1 & 8 & -1\\ -1 & -1 & -1 \end{bmatrix}\)

ecart type → 0

\(p(x) = { { 1 \over \sqrt{2π}θ } e^{- (x-m)^2 \over 2θ^2 } }\)

m : valeur moyenne → 0

\(p(x) = { { 1 \over \sqrt{2π}θ } e^{- x^2 \over 2θ^2 } }\)


### Projet 

avoir demarche recherche

expliquer les choix

pourquoi ça marche ou pas

ne pas ce contenter du sujet donné

1. Split and merge
2. Inpainting


3. Mosaïque
4. biometrie / Eigenfaces
5. Extraction de points caractériqtiques
6. Bibliothèque de traitement d'image
7. Fx (filtre un peu marrant)

- 01/03 : lancement 
- 05/04 : point d'avancement
- 26/04 : présentation (~10 min + question)

![](http://www.lgi2p.mines-ales.fr/~montesin/CoursPDF/corners_2010_04_15.pdf)


- implementation d'un detecteur de points

- correspondance de points
  - detecter les points interressent (FAST, HARRIS(variation rapide d'intensité))
  - les mettres en relation

- Comparaison d'image
	- test avec des spécification

- essayer de reconstruire une image en 3D avec plusieurs image 2D
	- tester avec 2 images différentes
	- puis une multitudes