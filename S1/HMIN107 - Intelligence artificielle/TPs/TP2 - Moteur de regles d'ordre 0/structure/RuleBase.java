package structure;

import java.util.ArrayList;

public class RuleBase {

	private ArrayList<Rule> rules; // l'ensemble des regles

	/**
	 * Constructeur : cree une base de regles vide
	 */
	public RuleBase()
	{
		rules = new ArrayList<Rule>();
	}
	

	/**
	 * ajoute une regle a la base de regles (sans verifier si elle y est deja)
	 * @param r regle a ajouter
	 */
	public void addRule(Rule r){
		rules.add(r);
	}
	
	/**
	 * retourne le nombre de regles
	 * @return le nombre de regles dans la base
	 */
	public int size()
	{ 
		return rules.size();
	}
	
	/**
	 * teste si la base est vide
	 * @return vrai si la base est vide
	 */
	public boolean isEmpty() 
	{
		return rules.isEmpty();
	}
	
	
	/**
	 * retourne la regle de rang i
	 * @param i le rang de la regle (debut a 0)
	 * @return la regle de rang i
	 */
	public Rule getRule(int i)
	{
		return rules.get(i);
	}
	
	
	/**
	 * retourne une description de la base de regles
	 * @return description de la base de faits
	 * @see java.lang.Object#toString()
	 */
	public String toString()
	{
		String s = "La base contient "+rules.size()+" regles :\n";
		for(int i=0;i<rules.size();i++)
			s+="\t Regle "+(i+1)+" : "+rules.get(i)+"\n";	
		return s;
	}
	

	
}
