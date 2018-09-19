var Turret = JsWarTurret.extend({

	action : function() {
		return this.idle(); 
	}

});


var WarTurret = new Turret();