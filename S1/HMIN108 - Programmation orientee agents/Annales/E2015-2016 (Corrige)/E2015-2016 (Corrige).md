# Exercice 1

### Question a)

**Agent cognitif** : n'a pas une representation explicite de sont environnement (fourmi)
**Agent reactif** : a une représentation de son environnement, de lui même, des autres agents et peut raisonner sur ses représentations. (humain)

### Question b)

**max-one-of agentset [reporter]** : renvoie un agent dont la valeur du reporter est maximum

### Question c)

l'erreur signifie qu'une tortue ne peut pas executer la procedure go, pour eviter cette erreur il faut changer le contexte d'execution en observer plutot que tortue

### Question d)

reporter signifie renvoyer en Netlogo comme (return en c)

### Question e)

un agent du point de vue des système multi agent est une entité plongé dans un environnemnet or en NetLogo un agent est autant ce qui composent l'environnement (patches) que ce qui peuple cette environnement (turtles)

### Question f)

pour s'orienter dans la direction du patch :

```set heading towards patch-at 10 10```

puis pour se deplacer :

```fd 1```

### Question g)

turtles-here renvoie toute les tortues (agents qui ne sont pas des patches) qui ce trouve au meme endroie que le patch courant

### Question h)

la case forever sert à repeter la procedure qui est associer au bouton à l'infini

### Question i)

avec uphill :

```uphill bidule```

avec max-one-of

```move-to max-one-of neighbors [bidule]```

### Question j)

cette expression renvoie nobody si aucun des 8 patches voisin du patches sur lequel ce trouve la tortue ne respecte les condition energie = 100 ou taille > 50, si un ou plusieurs de ces 8 patches respectent la condition l'expression renverra un de ces patches au hasard   

### Question k)

```netlogo
to-report reine-proche
	report min-one-of reines [distance myself] 
end
```

### Question l)

```netlogo
; On suppose que les taux sont en pourcentage

patches-own [parfum]

ask patche-at random-xcor random-ycor [pchit-pchit] 

to pchit-pchit
	set parfum parfum + 100
end

to evaporation
	set parfum parfum - (100 * taux-evap) / 100 
end

to go
	diffuse parfum taux-diffusion / 100
	ask patches [evaporation]
end
```

### Question m)

(à completer)

champ de potientiel est un signal qui ce diffuse dans un envirronement et qui permet de remonter jusqu'à ça source, il associe à chaque partie de l'environnement une valeur pour remonter jusqu'à ça source il suffit de suivre la valeur la plus forte

gradiant : variable qui represent les variations (la pente) du champ de potentiel il pointe dans la direction du taux d'augmentation le plus elevé. il permet de se dirigé verre le prochaine endroit ou la valeur du champ de potentiel est maximum

dans un système multi agent cela peut servir d'indice ou de trace laissé par un agent permettant de remonté jusqu'a lui !

on peut par exemple simuler le comportement d'un predateur qui remonte l'odeur laisser par une proie

### Question n)


### Question o)

# Exercice 2

```netlogo
; On suppose que les taux sont en pourcentage [0, 100]

patches-own [taille taille-max]

set taille-max 100

ask patches [
	set taille random (taille-max + 1) ; [0, 100]
]

to croitre
	if taille < taille-max and random-float 1 < proba-croissance [
		set taille taille * (100 + taux-croissance) / 100 
	]
end

to go
	ask patches [croitre]
	tick
end
```

# Exercice 3

### Question 1)

FSM  hierarchique permet de caractériser des modes comportementaux différents, exprimés de manière hierarchique

permet d'englober des etats dans des activités et de switcher entre plusieurs activités

necessite d'implementer 
	- une pile d'état pour revenir à l'état precedent
	- des mecanismes d'exceptions (il faut dépiler souvent dans certain cas)

### Question 2)

```netlogo
; FSM

; FSM hierarchique

```

### Question 3)

les reflexes permettent de gerer les actions elementaires que l'ont peut faire à chaque tick

```netlogo
ask turtles [
	reflexes
	deliberer
	run ctask
]
```

### Question 4)

(diagramme FSM)

### Question 5)

(diagramme FSM)

difference avec le nombre d'action effectué

### Question 6)

(diagramme de subsumption)

### Question 7)

```java
public abstract class WarExplorerBrainController extends WarExplorerBrain {

	private int baseAngle;

    public WarExplorerBrainController() {
        super();
        baseAngle = 0;
    }

    @Override
    public String action() {

		if (isBagFull())
			broadcastMessageToType(WarBase, "donne moi ta position");

		List<WarMessage> msgs = getMessages();
		
        for (WarMessage msg : msgs) {
            if (msg.getMessage().equals("base donne position")) {
                baseAngle = msg.getAngle();
            }
        }

        List<WarAgentPercept> percepts = getPercepts();

        for (WarAgentPercept p : percepts) {

        	if (!isBagEmpty()) {
        		if (p.getType() == WarBase && p.getDistance() < WarFood.MAX_DISTANCE_GIVE) {
					setNextAgentToGive(p.getId());
					return WarExplorer.ACTION_GIVE;
        		}
        	}

        	if (isBagFull()) {
				setHeading(baseAngle);
				return WarExplorer.ACTION_MOVE;
        	}

        	if (p.getType() == WarFood) {
				
				if (p.getDistance() < WarFood.MAX_DISTANCE_TAKE) 
					return WarExplorer.ACTION_TAKE;
				else
					setHeading(p.getAngle());
        	}
        }

        if (isBlocked())
            setRandomHeading();

        return WarExplorer.ACTION_MOVE;
    }
}
```