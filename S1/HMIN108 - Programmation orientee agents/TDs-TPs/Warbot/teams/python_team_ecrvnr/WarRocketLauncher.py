dico = {};
dico['attack'] = 0;
dico['fighting'] = 0;
dico['r'] = 0;
dico['angleBase'] = 0;
dico['followbase'] = 0;

def actionWarRocketLauncher():

	messages = getMessages();
	
	for mess in messages:
		if(mess.getMessage() == "enemyBase"):
			dico['attack'] = 1;
	
	
	if(dico['fighting'] == 1):
		percepts = getPercepts();
		if(not isReloaded()):
			return reloadWeapon();
		
		for percept in getPercepts():
			if(isEnemy(percept) and not percept.getType().equals(WarAgentType.WarFood)):
			
				dico['fighting'] = 1;
				setHeading(percept.getAngle());
				setTargetDistance(percept.getDistance());
				return fire();
		dico['fighting'] = 0;
		

	if(dico['attack'] == 0):
		if(dico['fighting'] == 0):
			for message in messages:
				if(message.getSenderType().equals(WarAgentType.WarBase) and message.getMessage() == "Count"):
					sendMessageToBases("rock", "");
				if(message.getSenderType().equals(WarAgentType.WarBase)):# and message.getMessage() == "attacked"):
					if(message.getDistance() > 70):
							
						setHeading(message.getAngle());
						return move();
					else:
						setHeading(message.getAngle() + 90);
						
			

		percepts = getPercepts();
		if(not isReloaded()):
			return reloadWeapon();
		for percept in percepts:
			if(isEnemy(percept) and not percept.getType().equals(WarAgentType.WarFood)):
				dico['fighting'] = 1;
				setHeading(percept.getAngle());
				setTargetDistance(percept.getDistance());
				return fire();
		dico['fighting'] = 0;
			
	else:
		for message in messages:
			if(message.getMessage() == "enemyBase"):
				if(message.getDistance() > 30):
					setHeading(message.getAngle());
				else: 
					setHeading(message.getAngle() + 90);
	
			
		
		percepts = getPercepts();
		if(not isReloaded()):
			return reloadWeapon();
		for percept in percepts:
			if(isEnemy(percept) and not percept.getType().equals(WarAgentType.WarFood)):
				setHeading(percept.getAngle());
				setTargetDistance(percept.getDistance());
				return fire();
		
		return move();
	
	
	if(isBlocked()):
		RandomHeading();
		
	return move();
	
