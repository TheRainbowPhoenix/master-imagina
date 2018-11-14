def actionWarLight():

	percepts = getPercepts();

	for percept in percepts:
		if (percept.getType().equals(WarAgentType.WarRocketLauncher) or percept.getType().equals(WarAgentType.WarLight) or percept.getType().equals(WarAgentType.WarHeavy)):
			if (isEnemy(percept)):
				setDebugString("Mode hunter")
				setHeading(percept.getAngle())

				if (isReloaded()):
					return fire()
				else :
					return reloadWeapon()
			else:
				setDebugString("No cible")

		elif (percept.getType().equals(WarAgentType.WarBase)):
				if (isEnemy(percept)):
					setDebugString("Mode hunter")
					setHeading(percept.getAngle())

					if (isReloaded()):
						return fire()
					else :
						return reloadWeapon()
				else:
					setDebugString("No cible")
		else:
			setDebugString("No cible")

	if (len(percepts) == 0):
		setDebugString("No cible")

	if(isBlocked()):
		RandomHeading()


	return move();
