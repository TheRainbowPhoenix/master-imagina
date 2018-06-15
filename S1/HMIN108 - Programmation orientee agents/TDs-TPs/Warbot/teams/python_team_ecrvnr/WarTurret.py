dico = {};
dico['enemy'] = 0;

def actionWarTurret():

	perceptsA = getPerceptsAllies();
	if(not haveNoTargets(perceptsA)):
		for perc in perceptsA:
			if(perc.getType().equals(WarAgentType.WarEngineer)):
				sendMessage(perc.getID(),"see",""); 


	if(dico['enemy'] == 0):
		setDebugString("watching");
		setHeading(getHeading() + 90);
		
	percepts = getPercepts();
	if(not isReloaded()):
		return reloadWeapon();
	for percept in percepts:
		if(isEnemy(percept) and not percept.getType().equals(WarAgentType.WarFood)):
			setDebugString("ennemy detected");
			dico['enemy'] = 1;
			setHeading(percept.getAngle());
			return fire();
			
	dico['enemy'] = 0;
	
	return idle();
	
