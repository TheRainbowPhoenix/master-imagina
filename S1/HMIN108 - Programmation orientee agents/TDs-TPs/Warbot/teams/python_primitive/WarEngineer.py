
def actionWarEngineer():
	percepts = getPercepts()

	if getPerceptsEnemiesWarBase():
		broadcastMessageToAll("EnemyBase","")

	if getPerceptsFood() and isNotBagFull():
		take()

	if isNotBagFull() and getHealth() < 0.75 * getMaxHealth():
		eat()

	if(getHealth() > 0.75 * getMaxHealth()):
		setDebugString("Creating tower")
		setNextBuildingToBuild(WarAgentType.WarTurret)
		return build()

	return move();
