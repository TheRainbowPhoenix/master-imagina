package structure;


/* Un atome est un symbole propositionnel. 
 * Remarque : le seul attribut d'un atome est la string correspondant au symbole 
 * Dans un but d'optimisation, on pourrait aussi associer à chaque symbole un entier distinct, 
 * et utiliser cet entier comme index dans diverses structures
 */

public class Atom
{
	private String symbol; // le symbole propositionnel
		
	/**
	 * @param s l'atome (
	 */
	public Atom (String s)
	{	
   		symbol = s;
	}
	
	/** overrrides equals from Object **/
	
	public boolean equals(Object b)
	{
		if (!(b instanceof Atom)) return false;
		return this.symbol.equals(((Atom)b).symbol);
	}
	
	/**
	 * @return le symbole propositionnel
	 */
	public String toString()
	{
		return this.symbol;
	}
	
}

