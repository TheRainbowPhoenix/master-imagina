package csp;

import java.io.BufferedReader;
import java.util.ArrayList;

public class ConstraintEq extends Constraint {

	ConstraintEq(BufferedReader in) throws Exception {
		super(in);
	}

	@Override
	public boolean violation(Assignment a) {

		for(String var : this.getVars()){
			if(!(a.getVars().contains(var)))
				return false;
		}

		String var1 = this.getVars().get(0);
		String var2 = this.getVars().get(1);

		return !a.get(var1).equals(a.get(var2));
	}

	@Override
	public String toString() {
		return "\n\t Eq "+ super.toString();
	}
}
