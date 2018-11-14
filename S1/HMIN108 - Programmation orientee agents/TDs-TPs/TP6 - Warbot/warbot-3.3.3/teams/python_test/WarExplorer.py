class SearchFoodState(object):
    @staticmethod
    def execute():
        setDebugString("SearchFoodState")
        if isBagFull():
            actionWarExplorer.nextState = GoHomeState
            return idle()

        actionWarExplorer.nextState = SearchFoodState
        percepts = getPerceptsFood()
        if percepts:
            #broadcastMessageToAgentType(WarAgentType.WarExplorer,"FoodHere","")
            if percepts[0].getDistance() < getMaxDistanceTakeFood():
                return take()
            else:
                followTarget(percepts[0])
                return move()
        else:
            for message in getMessages():
                if message.getMessage() == "FoodHere":
                    followTarget(message)
        return move()

class GoHomeState(object):
    @staticmethod
    def execute():
        setDebugString("GoHomeState")
        if getNbElementsInBag() == 0:
            # Transition vers l'état SearchFood
            actionWarExplorer.nextState = SearchFoodState
            return idle()
	    # Maintien de l'état
        actionWarExplorer.nextState = GoHomeState
        percepts = getPerceptsAlliesWarBase()
        # Cherche une base dans son champs de vision
        if percepts:
            if percepts[0].getDistance() < maxDistanceGive():
                giveToTarget(percepts[0])
                return give()
            else:
                followTarget(percepts[0])
                return move()
        # Cherche une base par les messages
        broadcastMessageToAgentType(WarAgentType.WarBase,"whereAreYouBase","")
        for mess in getMessages():
            if isMessageOfWarBase(mess):
                followTarget(mess)
                return move()
        return move()


class WiggleState(object):
    @staticmethod
    def execute():
        setDebugString("WiggleState")
        if (isBlocked()) :
            RandomHeading()
        return move();

def reflexes():
    if getPerceptsEnemiesWarBase():
        broadcastMessageToAll("EnemyBase","")
    if isBlocked():
        RandomHeading()
    return None


def actionWarExplorer():
    result = reflexes() # Reflexes
    if result:
        return result

    # FSM - Changement d'état
    actionWarExplorer.currentState = actionWarExplorer.nextState
    actionWarExplorer.nextState = None

    if actionWarExplorer.currentState:
        return actionWarExplorer.currentState.execute()
    else:
        result = WiggleTask.execute()
        actionWarExplorer.nextState = WiggleTask
        return result

# Initialisation des variables
actionWarExplorer.nextState = SearchFoodState
actionWarExplorer.currentState = None
