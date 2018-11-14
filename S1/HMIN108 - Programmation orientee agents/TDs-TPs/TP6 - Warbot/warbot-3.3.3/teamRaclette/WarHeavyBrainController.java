package teamRaclette;

import edu.warbot.agents.enums.WarAgentType;
import edu.warbot.agents.percepts.WarAgentPercept;
import edu.warbot.brains.brains.WarHeavyBrain;
import edu.warbot.brains.brains.WarRocketLauncherBrain;

public abstract class WarHeavyBrainController extends  WarHeavyBrain {


    public WarHeavyBrainController() {
        super();
    }

    @Override
    public String action() {

        for (WarAgentPercept wp : getPerceptsEnemies()) {

            if (wp.getType().equals(WarAgentType.WarBase) || 
            		wp.getType().equals(WarAgentType.WarRocketLauncher) || 
            		wp.getType().equals(WarAgentType.WarHeavy)  ) {

                setHeading(wp.getAngle());

                this.setDebugString("Attaque!");
                if (isReloaded())
                    return ACTION_FIRE;
                else if (isReloading())
                    return ACTION_IDLE;
                else
                    return ACTION_RELOAD;
            }
        }

        if (isBlocked())
            setRandomHeading();

        return ACTION_MOVE;
    }

}