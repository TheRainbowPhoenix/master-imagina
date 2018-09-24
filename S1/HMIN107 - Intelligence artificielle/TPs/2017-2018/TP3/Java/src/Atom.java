import java.util.ArrayList;

/**
 * Represents an atom as a predicate and a list of its arguments.
 */

public class Atom {
    private String predicate; //the atom's predicate
    private ArrayList<Term> args; //the atom's list of arguments, or terms

    /**
     * Cree un atome, avec ou sans termes
     *
     * @param s l'atome, soit reduit a un nom de predicat, soit de la forme
     *          predicat(liste de termes), ou les termes sont separes par des virgules
     */
    public Atom(String s) {
        args = new ArrayList<Term>();
        //if the atom has no terms
        if (s.charAt(s.length() - 1) != ')') {
            predicate = s;
        } else {
            int cpt = 0;
            String nomAtome = "";//to construct the label
            String nomTerme = "";//to construct the current term
            boolean constanteTerme;//is the current term a constant?

            while (s.charAt(cpt) != ')') {
                //get the atom's label
                while (s.charAt(cpt) != '(') {
                    nomAtome += s.charAt(cpt);
                    cpt++;
                }
                predicate = nomAtome;
                cpt++;//to skip the '('
                //loop through the atom's terms
                while (s.charAt(cpt) != ')') {
                    //get the label
                    while (s.charAt(cpt) != ',' && s.charAt(cpt) != ')') {
                        nomTerme += s.charAt(cpt);
                        cpt++;
                    }
                    //if the term is a constant
                    if (nomTerme.charAt(0) == '\'') {
                        constanteTerme = true;
                        nomTerme = nomTerme.substring(1, nomTerme.length() - 1);
                        //if it's a constant, remove the "'"
                    } else
                        constanteTerme = false;
                    Term t = new Term(nomTerme, constanteTerme);//create a new term
                    args.add(t); //add the new term if it didn't exist already
                    nomTerme = "";
                    if (s.charAt(cpt) == ',')
                        cpt++; //to skip the ','
                }
            }
        }
    }

    /**
     * clone() method for the Atom class.
     * @return A new atom that is a superficial copy of this.
     */
    @Override
    public Atom clone(){
        return new Atom(this.toString());
    }

    /**
     * Applies a homomorphism to this atom and returns the result.
     * @param assignment The homomorphism to apply
     * @return The result of the assignment (an atom containing only constants).
     */
    public Atom applyHomomorphism(Assignment assignment){
        Atom atom = this.clone();
        for(int i = 0 ; i < this.getArity() ; i++){
            if(atom.getArgI(i).isVariable()){
                atom.setArgI(i , new Term((Term) assignment.get(atom.getArgI(i).getLabel())));
            }
        }
        return atom;
    }

    /**
     * Ajoute le terme t a la liste de termes de l'atome, sans autre verification
     *
     * @param t le terme a ajouter
     */
    public void addTerm(Term t) {
        args.add(t);
    }

    /**
     * Returns the arity of this atom
     *
     * @return int arity of the atom
     */
    public int getArity() {
        return args.size();
    }

    /**
     * Returns the arguments of this atom
     *
     * @return list of arguments
     */
    public ArrayList<Term> getArgs() {
        return args;
    }

    /**
     * returns the i-th argument of this atom
     *
     * @param i the index of the wanted argument
     * @return Term Term at index i
     */
    public Term getArgI(int i) {
    		return args.get(i);
    }


    /**
     * modifie le ieme argument de l'atome
     *
     * @param i la position de l'argument
     * @param t le nouveau ieme terme
     */
    public void setArgI(int i, Term t) {
        args.set(i, t);
    }

    /**
     * @return le predicat de l'atome
     */
    public String getPredicate() {
        return predicate;
    }


    /**
     * @return vrai si l'atome est un symbole propositionnel (arite 0)
     */
    public boolean isProp() {
        return args.size() == 0;
    }


    /**
     * Teste l'egalite des predicats de deux atomes avec le label et l'arite
     * de l'atome
     *
     * @param r l'atome a tester par rapport a l'atome courant
     * @return vrai si les deux atomes ont meme predicat, faux sinon
     */
    public boolean equalsP(Atom r) {
        return (predicate.equals(r.predicate) && args.size() == r.args.size());
    }

    /**
     * Teste l'egalite de deux atomes (meme label et meme liste de termes)
     *
     * @param r l'atome a tester par rapport a l'atome courant
     * @return vrai si les deux atomes sont egaux, faux sinon
     */
    public boolean equalsA(Atom r) {
        if (!equalsP(r)) return false;
        for (int i = 0; i < args.size(); i++)
            if (!args.get(i).equals(r.args.get(i))) return false;
        return true;
    }

    /**
     * Retourne la chaine de caracteres de cet atome
     *
     * @return la chaine decrivant l'atome (suivant l'ecriture logique habituelle)
     */
    public String toString() {
        String s = predicate;
        if (args.size() > 0) {
            s += "(";
            for (int i = 0; i < args.size(); i++) {
                s += args.get(i);
                if (i < args.size() - 1) s += ",";
            }
            s += ")";
        }
        return s;
    }
}

