package myteam;

import edu.warbot.brains.WarBrain;

public abstract class WTask {
	WarBrain myBrain;
	
	abstract String exec(WarBrain bc);
}
