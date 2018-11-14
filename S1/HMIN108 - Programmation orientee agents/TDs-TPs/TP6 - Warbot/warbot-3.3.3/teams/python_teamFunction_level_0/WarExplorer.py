
def actionWarExplorer():

    percepts = getPercepts();

    for percept in percepts:
        if(percept.getType().equals(WarAgentType.WarFood)):
            if((percept.getDistance() < getMaxDistanceTakeFood()) and (not isBagFull())):
                setHeading(percept.getAngle());
                return take();
            elif (not isBagFull()) :
                setHeading(percept.getAngle());

    if (isBagFull()) :
        setDebugString("Bag full return base")

        percepts = getPerceptsAlliesByType(WarAgentType.WarBase);

        if((percepts is None) or (len(percepts) == 0)):
            #broadcastMessageToAll("whereAreYou", "");

            messages = getMessages();

            for message in messages :
                if(message.getSenderType() == WarAgentType.WarBase):
                    setHeading(message.getAngle());

            broadcastMessageToAgentType(WarAgentType.WarBase, "whereAreYou", "");

        else :
            base = percepts[0];

            if(base.getDistance() > maxDistanceGive()):
                setHeading(base.getAngle());
                return move();
            else:
                setIdNextAgentToGive(base.getID());
                return give();
    else :
        setDebugString("Chercher food");

    if (isBlocked()) :
        RandomHeading()

    return move()
