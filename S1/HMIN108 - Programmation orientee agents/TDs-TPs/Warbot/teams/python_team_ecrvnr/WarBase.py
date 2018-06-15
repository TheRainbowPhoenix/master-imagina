don= {}
don['toCreate']= 5;
don['nbrexp'] = 0;
don['nbrRocket'] = 0;
don['nbrLight'] = 0;
don['nbrHeavy'] = 0;
don['nbrKam'] = 0;
don['nbrEng'] = 0;


def actionWarBase():
	
	if(don['toCreate'] == 5):
		if(isAbleToCreate(WarAgentType.WarEngineer)):
			setNextAgentToCreate(WarAgentType.WarEngineer);
			don['toCreate'] = 0;
			return create();

	percepts = getPercepts();
	for percept in percepts:
		if(isEnemy(percept) and not percept.getType().equals(WarAgentType.WarFood)):
			broadcastMessageToAll("attacked","");

	don['nbrexp'] = 0;
	don['nbrRocket'] = 0;
	don['nbrLight'] = 0;
	don['nbrHeavy'] = 0;
	don['nbrKam'] = 0;
	don['nbrEng'] = 0;

	broadcastMessageToAll("Count","");
	
	messages = getMessages();

	for message in messages:
		if(message.getMessage() == "whereAreYou"):
			setDebugString("I'm here base PY");
			sendMessage(message.getSenderID(), "here", "");
		elif(message.getMessage() == "exp"):
			setDebugString(message.getMessage());
			don['nbrexp'] += 1;
		if(message.getMessage() == "rock"):
			don['nbrRocket'] += 1;
		if(message.getMessage() == "li"):
			don['nbrLight'] += 1;
		if(message.getMessage() == "heav"):
			don['nbrHeavy'] += 1;
		if(message.getMessage() == "kam"):
			don['nbrKam'] += 1;
		if(message.getMessage() == "eng"):
			don['nbrEng'] += 1;
			
			
			
	
	if(don['nbrRocket'] > 2 or don['nbrLight'] > 2 or don['nbrHeavy'] > 2 or don['nbrKam'] > 1):
		broadcastMessageToAll("go","");
		
	
	
	
	if(don['nbrexp'] <= 5):
		setNextAgentToCreate(WarAgentType.WarExplorer);
		don['toCreate'] = 0;
		return create();
	elif(don['nbrexp'] <= 3):
		don['toCreate'] = 0;
	elif(don['nbrRocket'] <= 2):
		don['toCreate'] = 1;
	elif(don['nbrLight'] <= 4):
		don['toCreate'] = 2;
	elif(don['nbrHeavy'] <= 3):
		don['toCreate'] = 3;
	elif(don['nbrKam'] <= 2):
		don['toCreate'] = 4;
		
	setDebugString(str(getMaxHealth()) + ' et ' + str(getHealth()) + " et " + str(don['nbrHeavy']) + " et " + str(don['toCreate']));
			
	if(don['toCreate'] == 0):
		if(isAbleToCreate(WarAgentType.WarExplorer) and getHealth() >= 3000):	
			setNextAgentToCreate(WarAgentType.WarExplorer);
			return create();
		else:
			return eat();
	
	elif(don['toCreate'] == 1):
		if(isAbleToCreate(WarAgentType.WarRocketLauncher) and getHealth() >= 3800):	
			setNextAgentToCreate(WarAgentType.WarRocketLauncher);
			return create();
		else:
			return eat();
	
	elif(don['toCreate'] == 2):
		if(isAbleToCreate(WarAgentType.WarLight) and getHealth() >= 3200):	
			setNextAgentToCreate(WarAgentType.WarLight);
			return create();
		else:
			return eat();
	
	elif(don['toCreate'] == 3):
		if(isAbleToCreate(WarAgentType.WarHeavy) and getHealth() >= 3500):	
			setNextAgentToCreate(WarAgentType.WarHeavy);
			return create();
		else:
			return eat();
	
	elif(don['toCreate'] == 4):
		if(isAbleToCreate(WarAgentType.WarKamikaze) and getHealth() >= 3000):	
			setNextAgentToCreate(WarAgentType.WarKamikaze);
			return create();
		else:
			return eat();
	
	elif(don['toCreate'] == 5):
		if(isAbleToCreate(WarAgentType.WarEngineer)):
			setNextAgentToCreate(WarAgentType.WarEngineer);
			return create();
		else:
			return eat();

	

	
	
		
	

	return idle();
