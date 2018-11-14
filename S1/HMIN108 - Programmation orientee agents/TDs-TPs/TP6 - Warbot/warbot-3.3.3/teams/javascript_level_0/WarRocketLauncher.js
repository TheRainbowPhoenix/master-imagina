var RocketLauncher = JsWarRocketLauncher.extend({

	action : function() { 

		if(this.isBlocked()) {
			this.randomHeading();
		}

		
		return this.move(); 

	}
});


var WarRocketLauncher = new RocketLauncher();