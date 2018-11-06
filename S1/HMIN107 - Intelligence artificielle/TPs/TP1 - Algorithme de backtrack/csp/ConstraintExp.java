package csp;

import java.io.BufferedReader;
import java.util.ArrayList;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

public class ConstraintExp extends Constraint {

	protected String expression;
	protected static ScriptEngineManager mgr = new ScriptEngineManager();


	ConstraintExp(BufferedReader in) throws Exception {
		super(in);
		expression = in.readLine().trim();
	}

	@Override
	public boolean violation(Assignment a) {

		for(String var : this.getVars()){
			if(!(a.getVars().contains(var))) return false;
		}

		String assigned_expression = expression;

		for(String var : this.getVars()){
			assigned_expression = assigned_expression.replace(var, a.get(var).toString());
		}

		boolean result = false;

		try {
			result = (boolean)mgr.getEngineByName("JavaScript").eval(assigned_expression);
		}
		catch (ScriptException e) { System.err.println("probleme dans: "+ assigned_expression); }

		//System.out.println(assigned_expression);

		return !result;
	}

	@Override
	public String toString() {
		return "\n\t Exp "+ super.toString();
	}
}
