
var Base = JsWarBase.extend({

	action : function() { 
		
		var messageAgents = this.getMessages();

		for (i = 0; i < messageAgents.size(); i++) {
			if(messageAgents.get(i).getMessage() == "whereAreYou") {
				this.setDebugString("I'm here base JS");
				this.sendMessage(messageAgents.get(i).getSenderID(), "here", "");
			}	
		} 
		
		return this.idle(); 
	}

});


var WarBase = new Base();