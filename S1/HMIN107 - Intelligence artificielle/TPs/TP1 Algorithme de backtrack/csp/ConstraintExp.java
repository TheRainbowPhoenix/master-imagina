package csp;

import java.io.BufferedReader;
import java.util.ArrayList;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

public class ConstraintExp extends Constraint {
	
	protected String expression;

	ConstraintExp(BufferedReader in) throws Exception {
		super(in);
		expression = in.readLine().trim();
	}
	
	@Override
	public boolean violation(Assignment a) {
		ArrayList<String> aVars = new ArrayList<String>(a.getVars());
		
		for(String var : this.getVars()){
			if(!(aVars.contains(var))) return false;
		}
				
		boolean result = false;
		
		String assigned_expression  = expression;
		for(String var : this.getVars()){
			assigned_expression = assigned_expression.replace(var, a.get(var).toString());
		}
		
		try {
			ScriptEngineManager mgr = new ScriptEngineManager();
			ScriptEngine engine = mgr.getEngineByName("JavaScript");
			result = (boolean)engine.eval(assigned_expression); 
		}
		catch (ScriptException e) { System.err.println("probleme dans: "+ assigned_expression); }
		
		//System.out.println(assigned_expression);
		
		return !result;
	}

}