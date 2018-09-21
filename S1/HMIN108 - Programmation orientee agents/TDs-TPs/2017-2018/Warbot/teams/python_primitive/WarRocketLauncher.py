
def actionWarRocketLauncher():

	percepts = getPerceptsEnemiesWarRocketLauncher()
	if percepts:
		for percept in percepts:
			setDebugString("Mode hunter rocket launcher")
			followTarget(percept)
			return shootTarget()

	percepts = getPerceptsEnemiesWarBase()
	if percepts:
		for percept in percepts:
			broadcastMessageToAll("EnemyBase","")
			setDebugString("Mode hunter base")
			followTarget(percept)
			return shootTarget()

	if (haveNoTarget()):
		setDebugString("No target")

	messages = getMessages()
	if messages:
		for m in messages:
			if m.getMessage() == "EnemyBase":
				setHeading(m.getAngle())

	if (haveTarget()):
		setDebugString("Target")

	if (isBlocked()):
		RandomHeading()

	return move();
