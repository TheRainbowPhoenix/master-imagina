# Warbot 3.3.3 (Novembre 2016)
- Tourelles de nouveau constructibles
- Equilibrage des unités (voir warbot_settings.yml)
- Zone de départ réduites de moitié (Les tourelles de départ sont donc plus efficaces)
- Les agents ne devraient plus apparaitre dans les murs
- Description des équipes affichées correctement après un reload

# Warbot 3.3.2 (Novembre 2016)
- Changement du versionnage (4.0.x -> 3.3.x)
- Les ingénieurs ne peuvent plus construire de base.

# Warbot 3.3.1 (Novembre 2016)
### Equilibrage
- Les caractéristiques des agents ont été rééquilibrées, consultez le fichier config/warbot_settings.yml pour avoir les nouvelles caractéristiques. Les changements ont été faits de sorte que chaque agent remplisse mieux son rôle, en amplifiant ses aventages et désavantages.

### Débug
- Le bouton Reload du launcher n'engendre plus de changement d'équipe. Vous pouvez l'utiliser pour recharger votre code python sans avoir besoin de relancer Warbot.
- Les fonctions suivantes fonctionnent désormais : 
 - myRoles
 - myGroups
 - getNumberOfAgentsInRole
- La fenêtre 2D.5 ne s'affiche plus au lancement.


# Warbot 3.3.0 (Avril 2016)
### Nouveaux modes de jeu 
- Récolte et survie
- Récolte et construction

### Nouveaux agents d'assaut
- Light (rapides, fragiles, cadence de tir élevée, peu de dégâts par coup, efficace contre les Lance-Roquettes)
- Heavy (lents, résistants, cadence de tir moyenne, dégâts moyens par coup, efficace contre les lights)
- Lance-Roquette (lents, fragiles, longue portée, dégâts de zone, tir en cloche au dessus des autres bots, dégâts élevés, efficace contre les Heavy)

### Graphismes 
- 2D.5 (2D isométrique avec sprites, work in progress)

### Autres
- Situation Loader (work in progress)

### Débug
- Messages en trop ou en moins : les messages sont maintenant bien reçus au tick suivant, et non aléatoirement en fonction de l'ordre d'activation des bots.

### Nouvelle documentation/tutoriel
- Documentation (work in progress)
- Tutoriel des fonctionnalités (work in progress)


# Warbot 3.2.6 (Décembre 2015)
### Mode de jeu
- Duel entre deux joueurs

### Agents 
- Base
- Explorateur
- Ingénieur
- Lance-Roquettes
- Tourelles
- Murs
- Boombots

### Graphismes
- 2D

### Autres
- Situation loader
