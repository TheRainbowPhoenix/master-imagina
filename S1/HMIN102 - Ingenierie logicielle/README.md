# HMIN102 - Ingenierie logicielle

## Sommaire [↺](#sommaire-)

* [Informations](#informations-)
  + [Examens](#examens-)
  + [Ressources](#ressources-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : 100% Exam

### Ressources [↺](#sommaire-)

- [Cours et TPs](http://www.lirmm.fr/~dony/enseig/IL/index.html)


schéma UML

# TP1-2 - Framework

- hierarchie de classe
- parametrage par spécialisation

# TP3-4 - Test unitaires

- Test Junit
- Test Mockito

# TP5 - Schema de conception

permet de décrire et generaliser des solutions logicielles

- Systeme de fichiers et répertoires
- Programmation graphique, boutons, canvas, boites de choix, panels
- quelle schéma?

# TP6 - Fabrique abstraite

Le but de ce patron de conception est d'isoler la création des objets de leur utilisation. On peut ainsi ajouter de nouveaux objets dérivés sans modifier le code qui utilise l'objet de base.

utile pour par exemple develloper sur plusieurs système d'exploitation sans changer le code de l'application

- Compiler

# TP7 - Decorateur

le decorateur offre une alternative plus souple a l'heritage et permet de rajouter/supprimer dynamiquement des nouvelles fonctionnalités à un objet

# TP8 - Commande 

Un objet commande sert à communiquer une action à effectuer, ainsi que les arguments requis. L'objet est envoyé à une seule méthode dans une classe, qui traite les commandes du type requis. Cela permet d'apporter des modifications aux commandes définies simplement dans la définition de la commande, et non dans chaque classe qui utilise la commande.

# TP8 - Feature 

# TP10 - Machine à etats

- classe etat
- classe contexte

La classe État permet via ses spécialisations de créer et gérer dynamiquement les comportements dont l'application aura besoin

# TP11 - Smalltalk et MVC

Permet de separer les données et elements logiques, l'affichage et la gestion des données d'une application 

M (Modèle) : Élément qui contient les données ainsi que de la logique en rapport avec les données
V (Vue) : Une vue contient des éléments visuels ainsi que la logique nécessaire pour afficher les données provenant du modèle
C (Controler) : Module qui traite les actions de l'utilisateur, modifie les données du modèle et de la vue

# Framework
# Abstract Factory
# Singleton
# Decorateur
# Adapteur
# Commande
# Bridge
# State
# MVC

**Exam**

le plus souvent on as
- framework dessin
- pattern decorateur sur dessin + UML
- pattern adapteur sur fonctionnalité dessin

ou alors

- framework avec prix HT, TTC
- avec variation de pattern dessus
	- Singleton
	- Commande 
	- Decorateur
	- State
	- Bridge