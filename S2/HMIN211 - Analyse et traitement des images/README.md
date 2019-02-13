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
