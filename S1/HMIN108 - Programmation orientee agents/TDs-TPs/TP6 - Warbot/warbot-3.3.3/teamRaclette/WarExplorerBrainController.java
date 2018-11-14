package teamRaclette;

import edu.warbot.agents.agents.WarExplorer;
import edu.warbot.brains.brains.WarExplorerBrain;

public abstract class WarExplorerBrainController extends WarExplorerBrain {

    public WarExplorerBrainController() {
        super();

    }

    @Override
    public String action() {

        if (isBlocked())
            setRandomHeading();
        return WarExplorer.ACTION_MOVE;
    }

}
