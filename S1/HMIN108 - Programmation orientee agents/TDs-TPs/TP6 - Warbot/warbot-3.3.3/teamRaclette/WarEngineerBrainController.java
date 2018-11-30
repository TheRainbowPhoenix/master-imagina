package teamRaclette;

import edu.warbot.agents.agents.WarEngineer;
import edu.warbot.brains.brains.WarEngineerBrain;

public abstract class WarEngineerBrainController extends WarEngineerBrain {


    public WarEngineerBrainController() {
        super();
    }

    @Override
    public String action() {

        return WarEngineer.ACTION_IDLE;
    }
}
