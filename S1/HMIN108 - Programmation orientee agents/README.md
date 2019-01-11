# HMIN108 - Programmation orientee agents

## Sommaire [↺](#sommaire-)

* [Informations](#informations-)
  + [Examens](#examens-)
  + [Ressources](#ressources-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : MAX(Exam, (60% Exam + 40% CC))

### Ressources [↺](#sommaire-)

- [Cours et TPs](http://www.lirmm.fr/~ferber/ProgAgent/index.html)
- [Html to Pdf Converter](https://www.sejda.com/fr/html-to-pdf)
- [Git Warbot](https://github.com/rascal222/TER-Warbot-2016)

### Introduction [↺](#sommaire-)

La programmation orienté agent utilise une métaphore social, on construit des programmes sous la forme d'entités autonomes en interactions.

Les systèmes multi-agenst (SMA) : 
- un ensemble c d'entités 
- plongées dans un environnement E
- un ensemble A d'entités inclus dans C
- un système d'action S permettant à des agents d'agir dans E
- un système de communication entre agents (envoie de messages, diffusion de signaux)
- une organisation O structurent l'ensemble des agents (notion de rôle/groupes)

Les agents physique ou logicielle  :
- agissent dans un environnement
- percevent et se representent partiellement leur environnement et les autres entités
- communiquent entre eux
- sont motivé par des tendances internes (buts, recherche de satisfaction)
- se conservent et se reproduisent
- jouent un role dans une organisation

Agents réactifs : ne disposent pas d'une représentation explicite de leurs environnement (fourmis)

Agents cognitifs : ont une représentation de leurs environnement d'eux mêmes et des autres agents et peuvent raisonner sur leurs représentations (humains)

ce qui est interressant avec la programmation agents c'est l'émergences de situation imprévus

### NetLogo

Un monde en 2D constitué de "patches" (portions de l'environnement) et "tortues" créatures qui peuple ce monde

#### Syntaxe

##### Procedures

to draw-carre[taille]
	pen-down
	repeat 4 [fd taille rt 90]
end

##### Fonctions

to-report absolute-value [number]
	ifelse number >= 0
	[ report number ]
	[ report 0 - number ]
end

##### Définitions variables globas

globals [max-energy]

##### Définitions attributs (tortues/patches)

turtles-own [energy speed]

##### Définitions variables locales

let r one-of turtles in-radius 5

##### Affectation de variables 

set energy 35


##### Donner des ordres

ask turtles [
	set color white
	setxy random-xcor random-ycor
]

ask patch 2 3 [ set pcolor green ]

##### Creer des tortues

create-turtles n [
	set color white
	set size 1.5 ;; easier to see
	set energy random (2 * max-energy)
	setxy random-xcor random-ycor
]

##### Deplacement

fd "distance", rt "angle", lt "angle"

##### S'orienté

set heading towards "patch"

face "patch"

##### Sélectionner un sous-ensemble de d'entités

turtles with [ color = red ]
patches with [ pxcor > 0 ]

##### Tester si agents vide

any? turtles in-radius 3

if v != nobody [  ] 

##### Compter le nombre d'agent dans un set

count turtles with [ color = red ]

##### Création de espèces

breed [ wolves wolf ]

##### Créé automatiqueme les procédures associées

create-<breed>
<breed>-own
<breed>-here
<breed>-at

##### Les listes

first, but-furst, last, item

fput, lput

length, empty?, member

remove, remove-item, replace-item

list, sentence, sublist

sort

### L'importance de l'environnement

objet avec l'attribut le plus petit

min-one-of reines in-radius 5 [distance  myself]

objet avec l'attribut le plus grand

max-one-of patches in-radius 8 [hauteur-herbe]

#### diffuser un attribut au patches 

diffuse <attribut> <coef>

uphill <attribut> (avance la tortue dans le patch dont la valeur de l'attribut est la plus grande)


### Architecture réactivees


pour programmer un agents il est plus simple d'utiliser des architectures speciales.

On part d'une notion d'états interne

FSM : finite state machine

Architecture de subsumption
Architecture de neuronale

#### Machines a états finis

Etats de l'automate : une activié de l'agent

Evenement : quelque chose qui se passe dans le monde extérieur (sert de déclancheur à lactivité)

Action : quelque chose que l'agent fait et aura pour conséquence de modifier la situation du monde et de produire d'autres évenements


##### implementation en utilisant un variable ctask

turtles-own [ctask]

to go
	ask turtles [ run ctask ]
end

Exemple voir implementation FSM termites


##### Implementation à l'aide de tables

Un interprete va sélectionner l'état courant dans la table et déclencher la chose à faire ensuite
Si pas de condition vérifié on demeure dans le meme état.

Etat courant | Action       | Condition                    | Etat suivant
en-fuite     | fuir-enemis  | sauf                         | patrouille
patrouille   | pattrouiller | menace ET ennemis-plus-fort  | en-fuite
--           |              | menace Et ennemis-moins-fort | en-attaque
en-attaque   | se-battre    | ennemis-vaincus              | patrouille
--           |              | ennemis-plus-fort            | en-fuite


### FSM hierarchique

Necessite d'implementer une pile

declaration de mode en fonction de condition ex si energie basse mode recuperation d'energie sinon mode en activité

turtles-own [stack ctask]

;; penser à initialiser l'agent dans le setup

set stack []
set cstack "proc-initiale"


;; en fait la proc initiale du mode
to go-mode [mode] 
	if ctask != mode [
		set stack fput ctask stack ;; push l'état ancien
		set ctask mode
	]
end

to return-mode
	ifelse empty? stack
	[;; s'arreter]
	[
		set cstak first stack
		set stack bf stack
	]
end

to go
	ask turtles [run ctask]
end

### FSM hiérarchique à reflexe

turtles-own [stack ctask]

;; penser à initialiser l'agent dans le setup

set stack []
set cstack "proc-initiale"


;; en fait la proc initiale du mode
to go-mode [mode] 
	if ctask != mode [
		set stack fput ctask stack ;; push l'état ancien
		set ctask mode
	]
end

to return-mode
	ifelse empty? stack
	[;; s'arreter]
	[
		set cstak first stack
		set stack bf stack
	]
end

to go
	ask turtles [run ctask]
end



Question TDs

Question TPs

Question Annales

# EXERCICE 1 : Questions generales

- 3 types d'architecture réactives

**donner la différence entre les termes, donner un exemple pour chaque**
	- Agent coginitif vs réactif => 
	- Agent d'un système multi-agent vs Agent NetLogo =>
	- FSM vs subsomption

**donner la signification du terme**
	- Reporter (NetLogo) => renvoyer une valeur
	- Champ potientiel **(x2)** =>
	- Gradient =>

**expliquer ce que fait la fonction, donner un exemple d'utilisation**
	- max-one-of (NetLogo) => 
	- any? (NetLogo)
	- empty? (NetLogo)
	- of (NetLogo)
	- turtle-here (NetLogo) =>
	- ask
	- breed
	- one-of neighbors with [energy = 100 and taille > 50] **(x2)** 

**donner la signification de l'erreur, comment l'eviter**
	- You can't use Go in a turtle context, because GO is observer-only => 

**expliquer l'utilité de cette element de l'interface**
	- case 'forever' dans un bouton Netlogo =>

**donner le code qui fait cette action**
	- Aller vers le patch de coordonnée px = 10, py = 10 =>
	- 2 manières d'aller vers le patch voisin ayant la plus grande valeur pour la variable 'bidule'
	- Retourner la reine la plus proche de l'abeille courante
	- Diffuser un signal de nom parfum avec un taux 'taux-diffustion' et une évaporation taux-evap (x2)
	- Fonction fibo(n)

**Question Warbot**
	- donner diagramme de séquence de communications d'un robot explorateur qui recrute exactement n lanceurs pour lancer une attaque contre une base ennemie (avec limite de temps si pas de réponses). **(x2)**
	- comment un explorer peut il communiquer à tous les lanceurs de missiles la position relative (en agnel et distance) d'une base ennemie qu'il vient d'apercevoir, donnez le code.
	- ecrire code qui permet a explorer de récuperer de la nourriture.

# EXERCICE 2 : Petit programme en NetLogo

- Représentez de l'herbe qui croit. (2016)

- Agent suiveur suit leader quand il le voit a une certaine distance
- procedure qui trouverTruc qui retourne un agent qui se trouve dans un rayon de n patches

# EXERCIE 3 : Architectures

- Différences FSM vs FSM hierarchique
- Donnez code générique (NetLogo) pour programmer FSM et FSM hiérarchique
- Donnez utilité des 'réflexes' dans une FSM ? est-il possible de les representer dans une FSM pure ? comment ?
- Donnez un diagramme comportemental de l'agent suivant (example agent avec ses etats)
- On ajoute un comportement a cette agent donnez le nouveau diagramme pour une FSM à réflexe, FSM hiérarchique, FSM pure. Y aura-t-il des différences ? **(x2)**
- Donnez diagrammes correspondant à l'architecture de subsomption décrivant cet agent **(x2)**
- On implemente ça avec Warbot, donnez le code de la fonction **action**  


# Introduction à NetLogo

breed [groupe_1 individu_1]
breed [groupe_2 individu_2]
....
breed [groupe_n individu_n]


patches-own [var_1 var_2 ... var_n]
turtles-own [var_1 var_2 ... var_n]

<breed>-own [var_1 var_2 ... var_n]

globals [var_1 var_2 ... var_n]

let var value
set var value

to setup 
	clear-all

	ask patches [...]

	create-groupe_1 number [
		setxy random-xcor random-ycor
		...
	]

	create-groupe_2 number [...]

	...

	create-groupe_n number [...]

	reset-ticks
end

to go
	ask groupe_1 [...]
	ask groupe_2 [...]
	...
	ask groupe_n [...]s
	tick
	update-plots
end

# Gerer l'environnement

Faire croître où décroître des plantes en fonction du temps en mode FSM.

```netlogo
patches-own [cpt cpt-init taille-plante taille-plante-max ctask]

to go
	ask patches [run ctask]
end

to-report wait-enough?
	ifelse number > 0 [
		set cpt cpt - 1
		report false
	][
		set cpt cpt-init
		report true
	]
end

to prairie-croit
	if wait-enough? {
	    set taille-plante taille-plante + 1
	    set pcolor scale-color green taille-plante 0 taille-plante-max
	    if taille-plante >= taille-plante-max [
	      set ctask "prairie-decroit"
	    ]
	}
end

to prairie-decroit
	if wait-enough? {
	    set taille-plante taille-plante - 1
	    set pcolor scale-color green taille-plante 0 taille-plante-max
	    if taille-plante <= 0 [
	      set ctask "prairie-croit"
	    ]
	}
end
```

# Reproduction

```netlogo
to reproduction
  if count animaux < nb-animaux-max and random-float 100 < taux-reproduction-animaux [
    hatch 1 ; Création d'un copie de moi-même
  ]
end
```

# Diffusion de signaux

```netlogo
to go
  ask lapins [run ctask]
  diffuse odeur taux-diffusion
  ask patches [colore]
  ask loups [run ctask]
  ask patches [evapore]
  tick
end

to colore
    set pcolor scale-color blue odeur 1 (odeur-max / 1.3)
end

to evapore
  set odeur odeur - (odeur * taux-evaporation)
end

to depose-odeur
    set odeur odeur + odeur-max
end

to remonte-odeur
  face max-one-of neighbors [odeur]
  fd vitesse
end
```

# Regroupement

```netlogo
to garde-espace-vitale
  let p min-one-of other breed in-radius espace-vitale [distance myself]
  if p != nobody [ ; Un agent de mon espèce est dans mon espace vital
    let temp heading
    set heading towards p
    bk espace-vitale - distance p ; Je m'eloigne de cette agent
    set heading temp
  ]
end
```

```netlogo
to vadrouille
  rt random 50
  lt random 50
  fd vitesse
end
```
