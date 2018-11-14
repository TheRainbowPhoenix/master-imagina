var Heavy = JsWarHeavy.extend({

	action : function() { 

		if(this.isBlocked()) {
			this.randomHeading();
		}

		
		return this.move(); 

	}
});


var WarHeavy = new Heavy();