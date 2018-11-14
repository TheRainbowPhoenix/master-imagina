from edu.warbot.agents.projectiles import WarBomb as Bomb

def actionWarKamikaze():
	# explode near a base
	for base in getPerceptsEnemiesWarBase():
		if base.getDistance() < Bomb.EXPLOSION_RADIUS:
			return fire()

	messages = getMessages()
	for message in messages:
		if message.getMessage() == "EnemyBase":
			face(message)

	if isBlocked():
		RandomHeading()
	return move()
