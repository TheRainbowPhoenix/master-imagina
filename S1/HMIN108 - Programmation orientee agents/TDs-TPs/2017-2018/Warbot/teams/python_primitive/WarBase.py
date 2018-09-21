
def actionWarBase():
	#Messages
	messages = getMessages()
	if messages:
		for message in messages:
			if(message.getMessage() == "whereAreYouBase"):
				reply(message, "here","")

	enemies = getPerceptsEnemiesByType(WarAgentType.WarRocket)
	if enemies:
		broadcastAll("Enemies near base","")


	if getHealth() >= 0.75 * getMaxHealth():
		return createRocketLauncher()


	if not isBagEmpty() and (getHealth() + getFoodHealthGiven()) < getMaxHealth():
		return eat()

	return BaseAction.ACTION_IDLE
