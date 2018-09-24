import java.io.BufferedReader;
import java.io.FileReader;
import java.util.*;

/**
 * Represents a Facts Base FB, as a list of valid predicates (facts).
 * Also includes a list of all terms present in the facts.
 */

public class FactBase {
	private HashMap<String, ArrayList<ArrayList<Term>>> facts;
	private ArrayList<Term> terms;

	/**
	 * Constructor for a facts base
	 *
	 * @param s Input from the text file under the form "a1;a2;...;ak"
	 */
	public FactBase(String s) {
		this.facts = new HashMap<String, ArrayList<ArrayList<Term>>>();
		this.terms = new ArrayList<Term>();
		String[] atoms = s.split(";");

		for (String atom : atoms) {
			Atom a = new Atom(atom);
			//if our factbase doesn't contain this predicate, create a new entry
			if (!(this.facts.containsKey(a.getPredicate()))) {
				ArrayList<ArrayList<Term>> terms = new ArrayList<ArrayList<Term>>();
				terms.add(a.getArgs());
				this.facts.put(a.getPredicate(), terms);
				//otherwise simply add the tuple
			} else {
				this.facts.get(a.getPredicate()).add(a.getArgs());
			}
			for (Term term : a.getArgs()) {
				if (!(this.containsTerm(term)))
					this.terms.add(term);
			}
		}
	}

	/**
	 * Constructor by copy for a facts base
	 *
	 * @param fb Another facts base to copy
	 */
	public FactBase(FactBase fb) {
		this.facts = new HashMap<String, ArrayList<ArrayList<Term>>>();
		this.terms = new ArrayList<Term>();
		//start by adding all facts from the original facts base to the saturated one
		for (String predicate : fb.getFacts().keySet()) {
			for (ArrayList<Term> fact : fb.getFacts().get(predicate)) {
				this.addFact(predicate, fact);
			}
		}
	}

	/**
	 * Returns a list of all the terms in the fact base
	 *
	 * @return list of all the terms
	 */
	public ArrayList<Term> getTerms() {
		return this.terms;
	}

	/**
	 * Returns the number of facts
	 * 
	 * @return number of facts
	 */
	public int getFactsNumber(){
		int nb = 0;
		for(String key : this.facts.keySet()){
			nb += this.facts.get(key).size();
		}
		return nb;
	}
	
	/**
	 * Tells us whether the facts base contains a certain term or not
	 *
	 * @param term the term to check
	 * @return true if the term exists in the facts base, false if not
	 */
	public boolean containsTerm(Term term) {
		for (Term t : this.terms)
			if (t.equals(term))
				return true;
		return false;
	}

	/**
	 * Checks whether the facts base contains a certain atom.
	 *
	 * @param atom the atom to check.
	 * @return true if the atom is in the facts base, false if not.
	 */
	public boolean containsAtom(Atom atom) {
		if(!(this.facts.keySet().contains(atom.getPredicate())))
			return false;
		if(!(this.facts.get(atom.getPredicate()).contains(atom.getArgs())))
			return false;
		return true;
	}

	/**
	 * Adds a new fact to the facts base.
	 *
	 * @param predicate the predicate of the fact.
	 * @param arguments the last of arguments.
	 */
	public void addFact(String predicate, ArrayList<Term> arguments) {
		if (!(this.facts.keySet()).contains(predicate))
			this.facts.put(predicate, new ArrayList<ArrayList<Term>>());
		if (!(this.facts.get(predicate).contains(arguments)))
			this.facts.get(predicate).add(arguments);
		for (Term term : arguments)
			if (!(this.terms.contains(term)))
				this.terms.add(term);
	}

	/**
	 * Checks whether the facts base contains a given list of atom (in no particular order)
	 *
	 * @param atoms the atoms to check
	 * @return true if the atoms are all in the facts base, false if not
	 */
	public boolean containsAtoms(ArrayList<Atom> atoms) {
		for (Atom atom : atoms) {
			if (!(this.containsAtom(atom)))
				return false;
		}
		return true;
	}

	/**
	 * Returns a list of facts (atoms with only constants)
	 *
	 * @return list of all facts
	 */
	public HashMap<String, ArrayList<ArrayList<Term>>> getFacts() {
		return this.facts;
	}

	/**
	 * Returns a string descripting the fact base
	 *
	 * @return String
	 */
	public String toString() {
		String s = new String();

		//print facts
		s += "Facts : { ";
		for (String predicate : this.facts.keySet()) {
			for (ArrayList<Term> atom : this.facts.get(predicate)) {
				String curAtom = predicate + "(";
				for (Term term : atom) {
					curAtom += term.toString() + ",";
				}
				curAtom = curAtom.substring(0, curAtom.length() - 1) + ")";
				s += curAtom + " ; ";
			}
		}
		s = s.substring(0, s.length() - 2) + "}";

		//print terms
		s += "\nTerms : < ";
		for (Term term : this.terms) {
			s += term.toString() + " , ";
		}
		s = s.substring(0, s.length() - 2) + ">";
		return s;
	}
}
