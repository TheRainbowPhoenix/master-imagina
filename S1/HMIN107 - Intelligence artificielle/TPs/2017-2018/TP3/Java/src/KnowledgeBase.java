import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Represents a knowledge base containing a facts base and a set of rules.
 * The first line of the buffer is expected to contain the facts, and each line after that
 * will contain one rule.
 */
public class KnowledgeBase {

    private FactBase factBase;//the facts base

    private ArrayList<Rule> rules;//the rules

    private FactBase saturatedFactBase;//the saturated facts base resulting from the forward chaining

    /**
     * The constructor of the knowledge base, from a buffered reader.
     * Once we have all facts and rules, we immediately saturate the facts base using
     * forward chaining, so that the knowledge base can take queries from the user.
     *
     * @param in a buffered reader to extract facts and rules from a text file.
     * @throws IOException if there's an error with readLine().
     */
    public KnowledgeBase(BufferedReader in) throws IOException {
        this.factBase = new FactBase(in.readLine());
        this.rules = new ArrayList<Rule>();
        String s;
        while ((s = in.readLine()) != null) {
            //System.out.println(s);
            rules.add(new Rule(s));
        }
        this.saturateFactBase();
    }

    /**
     * Simple getter for the facts base.
     *
     * @return the facts base.
     */
    public FactBase getFactBase() {
        return this.factBase;
    }

    /**
     * Simple getter for the saturated facts base.
     *
     * @return the saturated facts base.
     */
    public FactBase getSaturatedFactBase() {
        return this.saturatedFactBase;
    }

    /**
     * Simple getter for the list of rules.
     *
     * @return the list of rules.
     */
    public ArrayList<Rule> getRules() {
        return this.rules;
    }

    /**
     * Simple getter for a single rule.
     *
     * @param index the index of the required rule.
     * @return the i-th rule.
     */
    public Rule getRule(int index) {
        return this.rules.get(index);
    }

    /**
     * Converts the knowledge base to a string, to display the facts and the rules.
     *
     * @return a formatted String to print relevant information from the knowledge base
     */
    public String toString() {
        String s = "FACTS BASE: " + this.getFactBase().getFactsNumber() + " facts\n";
        s += this.factBase.toString() + "\n\n";
        s += "RULES\n";
        for (Rule rule : rules)
            s += rule.toString() + "\n";
        s += "\nSATURATED FACTS BASE: "+ this.getSaturatedFactBase().getFactsNumber() + " facts\n";
        s += this.saturatedFactBase;
        return s;
    }

    /**
     * Saturates the facts base by using forward chaining
     */
    public void saturateFactBase() {
        this.saturatedFactBase = new FactBase(this.factBase);
        while (true) {
            ArrayList<Atom> newFacts = new ArrayList<Atom>();
            for (Rule rule : this.rules) {//go through each rule
                Homomorphisms homomorphisms = new Homomorphisms(rule.getHypothesis(), this.saturatedFactBase);
                if (!(homomorphisms.getHomomorphisms() == null)) {
                    for (Assignment homomorphism : homomorphisms.getHomomorphisms()) {//for the current rule, check each homomorphism of its hypothesis in the saturated facts base
                        Atom atom = rule.getConclusion().applyHomomorphism(homomorphism);//apply the current homomorphism to the conclusion of the rule
                        if (!(this.saturatedFactBase.containsAtom(atom) || newFacts.contains(atom))) {//if the resulting atom isn't recorded yet (in newfacts or the facts base), add it to newfacts
                            newFacts.add(atom);
                        }
                    }
                }
            }
            if (newFacts.isEmpty())//if we haven't found any new fact after looping through all the rules, then we have saturated the facts base as much as possible
                return;
            else {//otherwise add all the new facts to the facts base and start again
                for (Atom newFact : newFacts) {
                    this.saturatedFactBase.addFact(newFact.getPredicate(), newFact.getArgs());
                }
            }
        }
    }
}
