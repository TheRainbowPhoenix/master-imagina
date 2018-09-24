import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;

/**
 * Represents all homomorphisms of a query Q in our facts base FB,
 * and lets us convert them to a network of constraints.
 * Contains a list of atoms (the query),
 * a facts base (that we run the query on),
 * and generates a list of homomorphisms as assignments.
 */

public class Homomorphisms {

    private ArrayList<Atom> query;

    private FactBase factBase;

    private ArrayList<Assignment> homomorphisms;

    /**
     * Constructor from a String and a facts base
     *
     * @param in       text string coding atoms as "a1;a2;...;ak"
     * @param factBase facts base
     */
    public Homomorphisms(String in, FactBase factBase) {
        this.query = this.fromString(in);
        this.factBase = factBase;
        this.homomorphisms = this.backtrackAll();
    }

    /**
     * Constructor from a list of Atoms and a facts base
     *
     * @param query    list of atoms
     * @param factBase facts base
     */
    public Homomorphisms(ArrayList<Atom> query, FactBase factBase) {
        this.query = (ArrayList<Atom>) query.clone();
        this.factBase = factBase;
        this.homomorphisms = this.backtrackAll();
    }

    /**
     * Generates an ArrayList of atoms from a String
     *
     * @param in String
     * @return list of atoms
     */
    private ArrayList<Atom> fromString(String in) {
        ArrayList<Atom> listAtoms = new ArrayList<Atom>();
        String[] atoms = in.split(";");
        for (String atom : atoms) {
            listAtoms.add(new Atom(atom));
        }
        return listAtoms;
    }

    /**
     * Returns all the homomorphisms of Q in FB
     *
     * @return A list of Assignments
     */
    public ArrayList<Assignment> getHomomorphisms() {
        return this.homomorphisms;
    }

    /**
     * Generates a network of constraints from this
     * For each variable appearing in the query, its domain is all the constants that appear at the same place in the same predicates in the facts base,
     * the constraints assert that each tuple of variables in an atom of the query must have the values of one tuple of constants in the same atom in the facts base.
     * For predicates of arity 1, we don't need constraints as the domain of the variable already handles it.
     * example : Q = { Eat(x,y) , Cruel(x)} ; FB = { Eat('Wolf','Goat') , Eat('Goat','Grass') , Cruel('Wolf') , Cruel('Fox') } =>
     * D(x) = < 'Wolf' , 'Fox' > ; D(y) = < 'Goat' , 'Grass' >
     * C1 = <x,y> | { ('Wolf','Goat') , ('Goat','Grass') }
     *
     * @return CstNetwork
     */
    private CstNetwork toConstraints() {
        CstNetwork cstNetwork = new CstNetwork();
        boolean closedQuery = true;

        for (Atom atom : this.query) {//go through each predicate of the query
            if (!(this.factBase.getFacts().keySet().contains(atom.getPredicate())))
                return null;
            ArrayList<String> variables = new ArrayList<String>();
            ConstraintExt cstExt;
            for (Term term : atom.getArgs()) {//go through the current predicate a first time to check if it has any variable
                if (term.isVariable()) {
                    closedQuery = false;
                    variables.add(term.toString());
                }
            }
            if (!(variables.isEmpty())) {//if the current atom has variables, start building a constraint from it
                cstExt = new ConstraintExt(variables);
                for (String variable : variables)
                    if (!(cstNetwork.getVars().contains(variable)))//add the new variables in the network of constraints
                        cstNetwork.addVariable(variable);
                for (ArrayList<Term> fact : this.factBase.getFacts().get(atom.getPredicate())) {//go through each instance of the same predicate in the facts base
                    if (atom.getArity() != fact.size())
                        break;
                    ArrayList<Object> tuple = new ArrayList<Object>();
                    for (int i = 0; i < atom.getArity(); i++) {//go through each term of the atom and the fact to compare them
                        if (atom.getArgI(i).isVariable()) {//if the current term in the atom is a variable, the query isn't closed
                            tuple.add(fact.get(i));
                        } else {//if it is a constant check that the current fact contains it at the right index, if not move on to the next fact immediately
                            if (!(atom.getArgI(i).equals(fact.get(i)))) {
                                tuple.clear();
                                break;
                            }
                        }
                    }
                    if (!(tuple.isEmpty())) {//if we found a fact that matches the current atom (in terms of all the constants being at the right indexes), add the new tuple to the constraint
                        for (int i = 0; i < tuple.size(); i++) {
                            if (!(cstNetwork.getDom(variables.get(i)).contains(tuple.get(i)))) {
                                cstNetwork.addValue(variables.get(i), tuple.get(i));
                            }
                        }
                        cstExt.addTuple(tuple);
                    }
                }
                if (cstExt.getArity() == 0) {//if we didn't find a single fact matching the current atom then the query can't be valid, so return null
                    return null;
                } else {
                    cstNetwork.addConstraint(cstExt);
                }
            } else {//if the current atom only has constants, ignore it if it is present in the facts base, or return null if it's not
                if (!(this.factBase.containsAtom(atom))) {
                    return null;
                }
            }
        }
        if (closedQuery) {//if the query is a closed question (all its predicates only contain constants), it can only be answered by "yes" or "no" by checking if it is a subset of the facts base, so return null
            return null;
        } else
            return cstNetwork;
    }

    /**
     * Calculates all homomorphisms of Q (query) in FB (facts base)
     *
     * @return list of homomorphisms
     */
    private ArrayList<Assignment> backtrackAll() {
        CstNetwork cstNetwork = this.toConstraints();
        if (cstNetwork == null)
            return null;
        else {
            CSP csp = new CSP(cstNetwork);
            //System.out.println(csp.CstNetwork);
            return csp.searchAllSolutions();
        }
    }

    /**
     * Simple method to convert this to a String to print
     *
     * @return textual representation of the homomorphisms of Q in FB
     */
    public String toString() {
        if (this.homomorphisms == null)
            if (this.factBase.containsAtoms(this.query))
                return "True";
            else
                return "The knowledge base doesn't let us infer this statement";

        if (this.homomorphisms.isEmpty())
            return "No homomorphisms found for this query";

        String s = new String();
        s += "Homomorphisms found :\n";
        for (Assignment assignment : this.homomorphisms)
            s += assignment.toString() + "\n";
        return s.substring(0, s.length() - 1);
    }
}
