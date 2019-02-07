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

### Pour la prochaine fois

- Definir des règles strict pour le jeux
	- 1 tir = 1 mort
	- vitesse porteur drapeau div/2
- Hierarchie de skills
- Idées d'entrainement pour les skills
- Lire les articles cité dans le sujet

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
	- RepulsionTarget (Sensor ennemi)

- Fight

- Defend

- ProtectFlagOwner

