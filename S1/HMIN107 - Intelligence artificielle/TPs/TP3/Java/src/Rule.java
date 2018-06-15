import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Represents a rule of the knowledge base.
 * Those are constructed from a String formatted as "h1;h2;...;hk=&gt;c",
 * with hi the i-th atom of the hypothesis and c the conclusion.
 * A static int is used to automatically number the Rules by order of creation.
 */
public class Rule {

    private ArrayList<Atom> hypothesis;//the list of atoms forming the hypothesis

    private Atom conclusion;//the conclusion

    private static int num = 0;//to automatically generate a new name for each rule

    private String name;//name of the rule

    /**
     * Constructs a rule from a String formatted as "h1;h2;...;hk=&gt;c",
     * with hi the i-th atom of the hypothesis and c the conclusion.
     *
     * @param s the input String
     */
    public Rule(String s) {
        this.hypothesis = new ArrayList<Atom>();
        num++;
        this.name = "R" + num;
        String[] parts = s.split("=>");
        String[] hAtoms = parts[0].split(";");
        for (String atom : hAtoms)
            this.hypothesis.add(new Atom(atom));
        this.conclusion = new Atom(parts[1]);
    }

    /**
     * Returns the list of atoms forming the hypothesis.
     *
     * @return A list of atoms.
     */
    public ArrayList<Atom> getHypothesis() {
        return this.hypothesis;
    }

    /**
     * Returns the atom forming the conclusion.
     *
     * @return an atom.
     */
    public Atom getConclusion() {
        return this.conclusion;
    }

    /**
     * Converts to String for printing.
     *
     * @return the rule under the form "h1;h2;...;hk=&gt;c"
     */
    public String toString() {
        String s = this.name + ": ";
        for (Atom atom : this.hypothesis) {
            s += atom.toString() + ";";
        }
        s = s.substring(0, s.length() - 1);
        s += " => " + this.conclusion.toString();
        return s;
    }

    /**
     * Simple getter for the name of this rule.
     *
     * @return The name of this string.
     */
    public String getName() {
        return this.name;
    }
}
