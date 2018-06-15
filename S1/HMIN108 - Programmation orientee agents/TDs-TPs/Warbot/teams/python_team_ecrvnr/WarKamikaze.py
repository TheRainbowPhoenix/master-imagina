dico = {};
dico['attack'] = 0;
dico['angleBase'] = 0;
dico['followbase'] = 0;

def actionWarKamikaze():
	messages = getMessages();
	
	for mess in messages:
		if(mess.getMessage() == "enemyBase"):
			dico['attack'] = 1;
	
	
	if(dico['attack'] == 0):
		messages = getMessages();
		for message in messages:
			if(message.getSenderType().equals(WarAgentType.WarBase) and message.getMessage() == "Count"):
				sendMessageToBases("kam", "");
			if(message.getSenderType().equals(WarAgentType.WarBase)):# and message.getMessage() == "attacked"):
				if(message.getDistance() > 50):
					setHeading(message.getAngle());
					return move();
				else:
					setHeading(message.getAngle() + 90);
				
		percepts = getPercepts();
		for percept in percepts:
			if(percept.getType().equals(WarAgentType.WarBase) and isEnemy(percept)):
				setHeading(percept.getAngle());
				return fire();
				
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
				broadcastMessageToAll("done","");
				return fire();
				
		return move();
				
	if(isBlocked()):
		RandomHeading();
		
	return move();
