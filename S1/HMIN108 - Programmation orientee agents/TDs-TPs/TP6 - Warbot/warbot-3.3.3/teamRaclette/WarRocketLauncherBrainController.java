package teamRaclette;

import edu.warbot.agents.enums.WarAgentType;
import edu.warbot.agents.percepts.WarAgentPercept;
import edu.warbot.brains.brains.WarRocketLauncherBrain;

public abstract class WarRocketLauncherBrainController extends WarRocketLauncherBrain {


    public WarRocketLauncherBrainController() {
        super();
    }

    @Override
    public String action() {

        for (WarAgentPercept wp : getPerceptsEnemies()) {

            if (wp.getType().equals(WarAgentType.WarBase)) {

                setHeading(wp.getAngle());
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