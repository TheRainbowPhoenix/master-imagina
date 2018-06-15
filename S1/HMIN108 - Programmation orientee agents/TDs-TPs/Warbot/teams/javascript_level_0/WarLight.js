var Light = JsWarLight.extend({

	action : function() { 

		if(this.isBlocked()) {
			this.randomHeading();
		}

		
		return this.move(); 

	}
});


var WarLight = new Light();