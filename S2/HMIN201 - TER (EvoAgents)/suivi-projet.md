# HMIN201 - TER (EvoAgents) : suivi de projet

## Rendez-vous du 17/12/2018 de 14h à 18h

### Preparation

#### Venir avec

- PC portable
- Eclipse
- Java 1.8

#### Lire

- MIND_181115 : L'article non publié qui explique le principe général. Lire chapitre 3 - 3.1 3.2, chapitre 4 en entier (le reste si vous voulez)

- EvoAgents_tutorial : Tutoriel du logiciel que vous allez utiliser qui implémente le principe de MIND. à lire en diagonale, c'est ce qu'on va appliquer pendant la réunion, vous retournerez à ce document comme référence.

### Resume

#### Hierarchie MIND et Agents

##### Creation du corps de l'agent

- Se deplacer dans **EvoAgentMind/src/evoagent2dsimulator/bot**
- Créer une classe qui héritera de **BotBody** (Ex: AgentBot)
- Définir dans le constructeur **public AgentBot(SimulationEnvironment2DMultiBot simEnv)**
	- La forme du corps avec la variable héritée **sd : FixtureDef**
	- Le comportement physique du corps avec la variable héritée **bd : BodyDef**
	- Les detecteurs de l'agent avec la variable héritée **sensors : HashMap<String, Sensor>**
	- Les actionneurs de l'agent avec la variable héritée **actuators : HashMap<String, Actuator>**.

##### Creation d'un environnement de simulation

- Se deplacer dans **EvoAgentMind/src/evoagent2dsimulator/experiments**
- Créer une classe qui héritera de **SimulationEnvironment** (Ex: EXP_GoToObjectAvoid)
- Définir l'environnement dans la methode **public void init()**. 

##### Creation de l'intelligence de l'agent

- Se deplacer dans **EvoAgentMind/Minds/**
- Créer un dossier dont le nom sera le nom de l'agent (Ex: AgentName)
- Définir dans le dossier AgentName:
	- Le Dossier **Skills** : contenant les competences de l'agent (codé en dur ou via réseau neural)
		- Créer un dossier du nom de la compétence et deplacez vous dedans (EX: GoToObjectAvoid)
		- Créer un fichier **.ades** du meme nom (EX: GoToObjectAvoid.ades)
		- Décrivez les actions à l'aide d'entrées sorties dans ce fichier **.ades** 
	- Le Dossier **Tasks** : contenant des fichiers **.simbatch** permettant de parametrer des simulations pour l'agent.
	- Le Fichier **AgentName.botdesc** : contenant la description de tous les capteurs et actionneurs de l'agent. 
	- Le Fichier **AgentName.vardesc** : contenant la description de toutes les variables de l'agent

##### Création de fonctions de récompense pour l'agent

- Se deplacer dans **EvoAgentMind/src/evoagent2dsimulator/experiments/elements**
- Créer une classe qui héritera de **RewardFunction** (Ex: RW_GoToObjectAvoid)
- Définir les recompenses à l'aide des methodes **public double computeRewardValue()** et **public void reset()**.

##### Execution du jar

- Se deplacer dans **EvoAgentMind/**
- Executer : java -jar ./EXE/EvoAgentApp_181125.jar ./Minds/Arnhulator/Tasks/CTF.simbatch

#### Plan du rapport

1. Rapport intro genéral
2. Etat de l'art
3. Plan des experiences
4. Réalisation des experiences
5. Choix dans la conception de la hierarchie MIND (logique, justifier)

## Rendez-vous du

### Preparation

- Var
- Sensor
- Skill
- Actuator

### Skills

#### Base

- AttractionEnv
- RepulsionEnv
- AttractionTarget (var)
- RepulsionTarget (var)
- Shoot

#### Complex

- ProcheEnv
	- AttractionEnv
	- RepulsionEnv

- ProcheTarget (var)
	- AttractionTarget (var)
	- RepulsionTarget (var)

- Hide
	- ProcheEnv
	- RepulsionTarget (< Sensor ennemi)

- Fight

- Defend

- ProtectFlagOwner

### Pour la prochaine fois

- Definir des règles strict pour le jeux
	- 1 tir = 1 mort
	- vitesse porteur drapeau div/2
- Hierarchie de skills
- Idées d'entrainement pour les skills
- Lire les articles cité dans le sujet

### Regle du jeu

- But
	- Recuperer drapeau ennemi et ramener à la base (1 point)
- Drapeau
	- 2 dans chaques bases
	- vitesse porteur drapeau div/2
	- drapeau reste sur place si laché (pas de teleportation)
- Map
	- Obstacle généré aléatoirement (espacé, pas de cuvette, )
	- 1 tir = 1 mort
- Capteur
	- Allié
	- Barycentre
		- Ennemi
		- Allié

#### Base

- AttractByEnv (sensor)
- RepulseByEnv (sensor)
- AvoidEnv (sensor)

- AttractByTarget (var)
- RepulseByTarget (var)
- AvoidTarget (var)

- ShootTarget (var)

#### Complex

- NearEnv (< SENSOR_ENV_DIST)
	- AttractByEnv (< SENSOR_ENV)
	- RepulseByEnv (< SENSOR_ENV)

- NearTarget (< VAR_TARGET_DIST)
	- AttractByTarget (< VAR_TARGET)
	- RepulseByTarget (< VAR_TARGET)

- Hide (> VAR_TARGET)
	- NearEnv (< SENSOR_ENV) 
	- RepulseByTarget (< VAR_TARGET)

- Flock (< SENSOR_ALLY_DIST, SENSOR_ENNEMI_DIST)
	- NearTarget  (< VAR_ALLY)
	- AvoidEnv    (< SENSOR_ENV)
	- AvoidTarget (< VAR_ENNEMI)

- Fight
	- ShootTarget
	- AvoidTarget

- ProtectTarget
	- Flock
	- Fight

- DefendBase
	- 

- CatchFlag

- Master
	- AvoidEnv
	- DefendBase
	- AttackEnnemiBase
	- PreventEnnemiStealFlag
	- StealEnnemiFlag

- StealEnnemiFlag
	- GotoEnnemiFlag
		- AttractByTarget
	- GotoBase 
		- AttractByTarget 
	- AvoidEnv

#### Hierarchie MIND V2

```
Avoid ≠ Repulse
les paramètres sont entre crochet []
je m'occupe pas de qui lit ou ecrit dans quoi
```

- Master
	- AvoidEnv < [SENSOR_ENV] → "Evite les obstacles" (Je le met en hauteur dans la hierarchie pour ne pas qu'il se retrouve dans toutes les skills)

	- Flocking < [GROUP_ALLY_DIRECTION, GROUP_ALLY_BARYCENTRE] → "Favorise les deplacements de groupes en general" (Possiblement à deplacer dans Surviving/Protecting/fighting)
		- AlignWithDirection  < [GROUP_ALLY_DIRECTION] → "Prend la meme direction"
		- NearTarget          < [GROUP_ALLY_BARYCENTRE] → "Reste à bonne distance"
			- AttractByTarget < [GROUP_ALLY_BARYCENTRE] → "Est repoussé"
			- RepulseByTarget < [GROUP_ALLY_BARYCENTRE]	→ "Est attiré"

	- Positioning < [GROUP_ALLY_BARYCENTRE, ENNEMI_FLAG_POS, ALLY_FLAG_POS, ...] → "Choisi un role en adequation avec la situation" (Les choix doivent être binaire pour que ça marche, je sais pas si c'est possible)
		- Defend < [ALLY_BASE] → "Protege la base allié, reste globalement dans la zone allié"
			- OrbitAroundTarget < [ALLY_BASE] → "Tourne autour de la base (roder)"
				- NearTarget    < [ALLY_BASE] 
				- KeepMoving → "Oblige le bot à bouger"
			- ???

		- Support → "Soutiens les attaquant ou defendeurs en fonction des besoins, milieu de jeu"
			- ???
			- ???

		- Attack < [ENNEMI_FLAG, ALLY_BASE] → "Essaye de prendre et ramener le drapeau ennemi, reste dans la zone du drapeau ennemi (tout le terrain)"
			- AttractByTarget < [ENNEMI_FLAG]
			- AttractByTarget < [ALLY_BASE]

	- Surviving → "Assure ta propre survie"
		- Hide                < [GROUP_ALLY_RATIO]
			- NearEnv         < [SENSOR_ENV]
			- RepulseByTarget < [ENNEMI]
		- ???

	- Protecting → "Protege les membres de ton equipe" (surtout le porteur de drapeau)
		- AttractByTarget < [ALLY_FLAG_OWNER]
		- Flocking 
		- ???

	- Fighting → "Combat l'ennemi"
		- ShootTarget < [ENNEMI]
		- AvoidTarget < [ENNEMI]

