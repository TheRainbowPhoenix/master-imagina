dico= {};
dico['turret'] = 0;
dico['counter'] = 0;
dico['food'] = 0;

def actionWarEngineer():
	dico['turret'] = 0;
	dico['counter'] += 1;
	
	if(dico['food'] == 0):
		setDebugString("Looking for food to place a turret");
		for percept in getPercepts():
			if(percept.getType().equals(WarAgentType.WarExplorer) and not isEnemy(percept)):
				setHeading(percept.getAngle());
			elif((percept.getType().equals(WarAgentType.WarFood) or isEnemy(percept))):
				setNextBuildingToBuild(WarAgentType.WarTurret);
				dico['food'] = 1;
				return build();
		if(isBlocked()):
			RandomHeading();
		return move();
	
	messages = getMessages();
	for mess in messages:
		if(mess.getMessage() == "Count"):
			sendMessageToBases("eng", "");	
		if(mess.getSenderType().equals(WarAgentType.WarBase)):
			base = mess;
			if(mess.getDistance() > 100):
				setHeading(mess.getAngle());
				return move();
			else:
				setHeading(mess.getAngle() + 90);
		if(mess.getMessage() == "see"):
			dico['counter'] = 0;
			dico['turret'] = 1;
			
	
	if(dico['turret'] == 0 and dico['counter'] > 5):
		if(getHealth() > 1300 and base.getDistance() > 20):
			setDebugString("creating tower" + str(getHealth()) + "  " + str(getMaxHealth()));
			setNextBuildingToBuild(WarAgentType.WarTurret);
			return build();
		elif(base.getDistance() <= 20):
			setHeading(mess.getAngle() + 90);
		else:
			setDebugString("wait for food" + "  " + str(getNbElementsInBag()));
			broadcastMessageToAll("needfood","");
			if(not isBagEmpty()):
				return eat();
			return idle();
	else:
		setDebugString("nope" + str(getHealth()) + "  " + str(getMaxHealth()) + "  " + str(getNbElementsInBag()));
	
	
	if (isBlocked()):
		RandomHeading();

	return move();
