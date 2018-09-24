/**
 * Represents and lets us manipulate logic terms as variables or 'constants'.
 */

public class Term {
    private String label;//le nom du terme (par exemple : x, 'toto')
    private boolean constant;//vrai si le terme courant est une constante, faux sinon (c'est une variable)

    /**
     * @param n le label du terme
     * @param c boolean qui indique si le terme est une constante ou pas (et alors c'est une variable)
     */
    public Term(String n, boolean c) {
        label = n;
        constant = c;
    }

    /**
     * Copy constructor from another term.
     *
     * @param t Term to construct from.
     */
    public Term(Object t) {
            this.label = ((Term) t).getLabel();
            this.constant = ((Term) t).isConstant();
    }

    /**
     * Indique si le terme est une constante
     *
     * @return vrai si le terme est une constante, faux sinon
     */
    public boolean isConstant() {
        return constant;
    }

    /**
     * Indique si le terme est une variable
     *
     * @return vrai si le terme est une variable, faux sinon
     */
    public boolean isVariable() {
        return !constant;
    }

    /**
     * Accesseur en lecture
     *
     * @return le label du terme
     */
    public String getLabel() {
        return label;
    }

    /**
     * Teste l'egalite du terme t et du terme courant (constante, label)
     *
     * @param t le terme a tester
     * @return vrai si t et le terme courant sont egaux, faux sinon
     */
    @Override
    public boolean equals(Object t) {
        if (t == null) return false;
        if (t == this) return true;
        if (!(t instanceof Term)) return false;
        Term other = (Term) t;
        return (constant == other.constant && label.equals(other.label));
    }

    /**
     * Compare le label du terme t et du terme courant
     *
     * @param t le terme a tester
     * @return entier negatif, nul ou positif selon que le label du terme courant est avant, identique, apres le terme t dans l'ordre lexicographique. Attention on ne s'occupe pas de savoir si les termes sont des variables ou des constantes
     */
    public int compareTo(Term t) {
        return (label.compareTo(t.label));
    }

    /**
     * Retourne la chaine de caracteres de ce terme
     *
     * @return la chaine decrivant le terme
     */
    public String toString() {
        if (constant) return "'" + label + "'";
        else return label;
    }
}
