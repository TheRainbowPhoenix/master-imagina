dico = {}
dico['isGivin'] = 0;
dico['poorEnv'] = 0;
dico['needtogo'] = 0;
dico['giveBase'] = 0;
dico['giveEng'] = 0;
dico['angleBase'] = 0;
dico['followbase'] = 0;

def actionWarExplorer():

	dico['giveBase'] = 1;
	dico['giveEng'] = 0;

	messages = getMessages();
	for mess in messages:
		if(mess.getMessage() == "go"):
			dico['needtogo'] = 1;
		if(mess.getMessage() == "enemyBase"):
			setHeading(mess.getAngle());
		if(mess.getMessage() == "needfood"):
			dico['giveEng'] = 1;
			dico['giveBase'] = 0;

	

	if(dico['needtogo'] == 1):
		setDebugString("Searchin enemy base");
		percepts = getPerceptsEnemies();
		if(haveNoTargets(percepts)):
			if (isBlocked()) :
				RandomHeading()

				return move();
		else:
			for percep in percepts:
				if(percep.getType().equals(WarAgentType.WarBase)):
					broadcastMessageToAll("enemyBase","");
					return idle();



	


	if(getNbElementsInBag() > 0 and isNotBagFull()):
		dico['poorEnv'] += 1;
	
	

	for message in messages:
		if(message.getSenderType().equals(WarAgentType.WarBase) and message.getMessage() == "Count"):
			sendMessageToBases("exp", "");


	if getPerceptsEnemiesWarBase():
		broadcastMessageToAll("EnemyBase","")


	for percept in getPerceptsFood():
		sendMessageToExplorers("FoodFound","")
		setDebugString("View Food");
		if(pickableFood(percept) and isNotBagFull()):
			setDebugString("Take food")
			followTarget(percept)
			return take()
		elif (isNotBagFull()):
			followTarget(percept)
			
	if(isBagEmpty()):
		dico['isGivin'] = 0;
		dico['poorEnv'] = 0;

	if(dico['giveBase'] == 1):
		if(dico['isGivin'] == 0):
			if(isBagFull() or dico['poorEnv'] > 200):
				setDebugString("Bag full return base");

				__percept = getPerceptsAlliesWarBase();

				if(haveNoTargets(__percept)):
					for message in getMessages():
						if(isMessageOfWarBase(message)):
							followTarget(message)


					sendMessageToBases("whereAreYou", "");

				else :
					base = __percept[0];

					if(isPossibleToGiveFood(base)) :
						dico['isGivin'] = 1;
						giveToTarget(base);
						return give();
					else:
						followTarget(base);
						
			else:
				setDebugString("Look for food" + str(getNbElementsInBag()) + str(dico['poorEnv']) + "  " + str(dico['giveEng']));

		else:
			__percept = getPerceptsAlliesWarBase();
			base = __percept[0];
			giveToTarget(base);
			return give();

	elif(dico['giveEng'] == 1):
		if(dico['isGivin'] == 0):  
			if(isBagFull() or dico['poorEnv'] > 200):
				setDebugString("Bag full/poor env give eng");

				__percept = getPerceptsAllies()
				if(not haveNoTargets(__percept)):
					for perc in __percept:
						if(perc.getType().equals(WarAgentType.WarEngineer)):
							if(isPossibleToGiveFood(perc)):
								dico['isGivin'] = 1;
								giveToTarget(perc);
								return give()
							else: 
								setHeading(perc.getAngle());
						else:
							for mess in getMessages():
								if(mess.getMessage() == "needfood"):
									setHeading(mess.getAngle());


		else:
			__percept = getPerceptsAllies();
			for perc in __percept:
				if(perc.getType().equals(WarAgentType.WarEngineer)):
					giveToTarget(perc);
					return give()

	messages = getMessages();
	for message in messages:
		if(message.getMessage() == "FoodFound" and (haveNoTargets(getPerceptsFood())) and (not isBagFull())):
			setHeading(message.getAngle());

	if (isBlocked()) :
		RandomHeading()

	return move();

