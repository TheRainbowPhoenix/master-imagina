var Explorer = JsWarExplorer.extend({

	action : function() { 

		percepts = this.getPercepts();

		var baseEnum = edu.warbot.agents.enums.WarAgentType.WarBase;

		//println(baseEnum);

		for (i = 0; i < percepts.size(); i++) { 
			//println("type percept");
			//println(percepts.get(i).getType());
			//println(this.getFoodType());
			if(percepts.get(i).getType() == this.getFoodType()) {
				//println("food view");
				if((percepts.get(i).getDistance() < this.getMaxDistanceTakeFood()) && (!this.isBagFull())) {
					this.setHeading(percepts.get(i).getAngle());
					return this.take();
				}
				else if (!this.isBagFull()) {
					this.setHeading(percepts.get(i).getAngle());
				}
			}
		}

		if(this.isBagFull()) {
			this.setDebugString("Bag full return base");

			/*perceptsBase = null;

			for (j = 0; j < percepts.size(); j++) { 
				if(percepts.get(i).getType == "WarBase") {
					perceptsBase = percepts.get(i);
				}
			}
			
			

			if(perceptsBase == null) {
				messageAgent = this.getMessages();
				mes = null;

				if(messageAgent != null) {
					for (i = 0; i < messageAgent.size(); i++) {
						mes = messageAgent.get(i);
						if(mes.getSenderType() == "WarBase") {
							mes = null;
						}
					} 
				}
				if(mes != null) {
					this.setHeading(mes.getAngle());
				}

				this.broadcastMessageToAgentType(baseEnum, "whereAreYou", "");
				
			}
			else {
				base = perceptsBase;

				if(base.getDistance() > this.maxDistanceGive()) {
					this.setHeading(base.getAngle());
					return this.move();
				}
				else {
					this.setIdNextAgentToGive(base.getID());
					return self.give();
				}
			}*/
		}
		else {
			this.setDebugString("Chercher food");
		}


		if(this.isBlocked()) {
			this.randomHeading();
		}

		return this.move(); 

	}
});


var WarExplorer = new Explorer();
