package structure;

import java.util.ArrayList;
import java.util.StringTokenizer;

public class Rule
{
	private ArrayList<Atom> hypothesis;//l'hypothese : une liste d'atomes
	private Atom conclusion;//la conclusion : un atome

	/**
	 * Constructeur
	 * @param strRule la regle, sous forme  sous forme textuelle ; cette forme est 
	 * "atome1;atome2;...atomek", ou les (k-1) premiers atomes forment l'hypothese,
	 * et le dernier forme la conclusion
	 */
	public Rule(String strRule)
	{
		hypothesis = new ArrayList<Atom>();		
		StringTokenizer st = new StringTokenizer(strRule,";");
   		while(st.hasMoreTokens())
   		{
   			String s = st.nextToken(); // s represente un atome
   			Atom a = new Atom(s);
   			hypothesis.add(a);//ajout de a a la liste des atomes de l'hypothese (pour l'instant, on ajoute aussi celui de la conclusion)
   		}
   		// on a mis tous les atomes crees en hypothese
   		// il reste a tranferer le dernier en conclusion
		conclusion = hypothesis.get(hypothesis.size()-1);
		hypothesis.remove(hypothesis.size()-1);
	}
	
	/**
	 * accesseur a l'hypothese de la regle
	 * @return l'hypothese de la regle
	 */
	public ArrayList<Atom> getHypothesis() 
	{
		return hypothesis;
	}
	
	/**
	 * retourne la ieme atome de l'hypothese
	 * @param i le rang de l'atome a retourner (debut a 0)
	 * @return le ieme atome de l'hypothese
	 */
	public Atom getAtomHyp(int i) 
	{
		return hypothesis.get(i);
	}

	
	/**
	 * accesseur a la conclusion de la regle
	 * @return l'atome conclusion de la regle
	 */
	public Atom getConclusion()
	{
		return conclusion;
	}
	
	/**
	 * retourne une description de la regle
	 * @return la chaine decrivant la regle (suivant l'ecriture habituelle)
	 */
	public String toString()
	{
		String s="";
		for(int i=0;i<hypothesis.size();i++)
		{
			s+=hypothesis.get(i);	
			if(i<hypothesis.size()-1) s+=" ; ";
		}
		s+=" --> ";  
		s+=conclusion;
		return s;
	}

	
}
