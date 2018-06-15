import java.io.BufferedReader;
import java.util.ArrayList;

/**
 * Represents equality constraints as a pair of variables that must be equal.
 */


public class ConstraintEq extends Constraint {

    ConstraintEq(BufferedReader in) throws Exception {
        super(in);
    }

    @Override
    public boolean violation(Assignment a) {
        ArrayList<String> aVars = new ArrayList<String>(a.getVars());
        for (String var : this.getVars()) {
            if (!(aVars.contains(var))) return false;
        }
        String var1 = this.getVars().get(0);
        String var2 = this.getVars().get(1);

        if (!(a.get(var1).equals(a.get(var2))))
            return true;
        else
            return false;
    }

}
