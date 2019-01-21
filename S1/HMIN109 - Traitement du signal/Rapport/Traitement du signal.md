# Traitement du signal

consignes :
	- montrer resultat d'experimentation
	- exliquer :
		- les constation de nos TPs
		- comment crééer un son avec la transformé de fourier
		- comment on applique les filtres sur des son
			- différences des resultats en fonction son crée vs vrai son enregistré
		- les difficulté rencontrées

à ne pas faire :
	- montrer de code
	- recopier le sujet ou des pages en lignes

notation :
	- originalité
	- aspect scientifique

rendu : olivier.strauss@umontpellier.fr



Transformé de fourier direct/inverse

tester si le son est identique après avoir fais les deux transformations

filtre butterworth en fonction d'un alpha : (en fonction de la freq d'ech et de la freq de coupure désirée)

tester fitrer son de haute et basse frequences

realiser filtre ne laissant passer que les hautes fréquences
essayer de retirer note selectivement

tester avec son


à chaque fois on réalises les opérations sur des sons crées, réel, gammes, etcs..

comparer les methodes de filtrages
	- butterworth
	- tchebychev
	- bessel
	- elliptique

selon plusieurs critères
	- selectivité

montrer la deformation des sons selon la methodes
	- creation de son en ajoutant des raies 
	- modification de l'espace temporelle a partir de l'espace frequenciel (ajouter des raies au pif)


# 1 Introduction

On présente dans l'introduction l'objectif du travail pratique ainsi que les notions nécessaires à la compréhension de l'étude : connaissances actuelles sur le sujet (avec références bibliographiques), définitions des mots nouveaux ou complexes utilisés, rappel ou mise au point des notions nécessaires à la compréhension du rapport par un étudiant de même âge mais non-initié.

2 Matériel et méthode

2.1 Matériel

On y décrit de façon détaillée le matériel utilisé lors de l'expérimentation, y compris la nature du sujet d'expérimentation et, le cas échéant, le lieu d'observation.

2.2 Méthode

On y fait la description du déroulement précis des différentes phases de l'expérience et du protocole expérimental.

3 Hypothèse de travail

L'énoncé de l'hypothèse de travail décrit la façon dont on s'attend à voir se dérouler l'expérience, donne le(s) résultat(s) escompté(s).
Tout le génie des chercheurs consiste à imaginer, en fonction d'expériences antérieures, de nouvelles hypothèses assez plausibles et fondées pour avoir une chance significative d'être vérifiées par l'expérience.

4 Résultats

4.1 Résultats bruts

On présente ici les résultats tels qu'ils ont été obtenus, de façon claire, ordonnée et logique (selon une liste chronologique de données obtenues, en tableau, etc.).

4.2 Résultats transformés

Dans certains cas, les données doivent être traitées : un graphique représentant une fonction de la variable observée, un calcul statistique... constituent des exemples de traitement des données brutes. La transformation utilisée doit être la plus apte à rendre les résultats facilement interprétables.

5 Conclusion

La conclusion correspond à l'adéquation ou l'inadéquation avec l'hypothèse de travail. On doit expliquer comment l'hypothèse de travail est vérifiée ou non d'après l'observation des résultats, dans les cas où cela pourrait ne pas paraître évident.

6 Discussion

La discussion dépasse la simple vérification de l'hypothèse de travail. On tente d'expliquer ici, si besoin est, pourquoi l'expérience a réussi ou échoué. La discussion présente les perspectives et orientations possibles des recherches futures ou du renouvellement de l'expérience dans de nouvelles conditions ou accompagnée d'une nouvelle hypothèse.




**Produit de convolution** : Effectuer le produit de convolution de deux fonctions signifie réaliser une moyenne de ces deux fonctions. A ce titre, il intervient notamment en électronique dans l'écriture des filtres passe-bande. Il est très utilisé en mathématiques pour approximer et régulariser des fonctions. 