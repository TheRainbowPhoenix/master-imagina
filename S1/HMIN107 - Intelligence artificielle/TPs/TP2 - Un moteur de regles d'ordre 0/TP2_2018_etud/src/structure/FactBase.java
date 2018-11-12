package structure;

import java.util.*;

/**
 * @author graphikTeam
 *
 */
public class FactBase
{
	private ArrayList<Atom> atoms; // l'ensemble des atomes
	
	/**
	 * Constructeur : cree une base de faits vide
	 */
	public FactBase()
	{
		atoms = new ArrayList<Atom>();
	}
	
	/**
	 * Constructeur : initialise avec une chaine de caracteres correspondant a des atomes
	 * @param factList : les faits, passes sous la forme "atom1;...;atomk"
	 */
	public FactBase(String factList)
	{
		this(); // appel au constructeur sans paramètres
		createFactBase(factList);	
	}
	
	/**
	 * Remplit la base de faits avec la chaine de caracteres passee en parametres
	 * @param factList: les faits, passes sous la forme "atom1;...;atomk"
	 */
	private void createFactBase(String factList)
	//Prerequis : le format de la base de faits est suppose correct
   	{
   		StringTokenizer st = new StringTokenizer(factList,";");
   		while(st.hasMoreTokens())
   		{
   			String s = st.nextToken(); // s represente un atome
   			Atom a = new Atom(s);
   			atoms.add(a);// on ajoute un atome a la liste des atomes
   		}
   	}
	


	/**
	 * Ajoute des atomes a la base de faits s'ils n'apparaissent pas deja
	 * @param atomList la liste d'atomes a ajouter (seuls ceux n'apparaissant pas dans la base seront ajoutes)
	 */
	public void addAtoms(ArrayList<Atom> atomList)
	{
		for(int i=0;i<atomList.size();i++)
		{ Atom a = atomList.get(i);
		  if (!belongsAtom(a)) addAtomWithoutCheck(a);
		}
	}
	
	/**
	 * Ajoute un atome a la base de faits (meme si il y est deja) 
	 * @param a l'atome a ajouter 
	 */
	public void addAtomWithoutCheck(Atom a)
	{
		atoms.add(a);
	}

	/**
	 * retourne la liste des atomes de la base de faits
	 * @return la liste des atomes de la base de faits
	 */
	public ArrayList<Atom> getAtoms() 
	{
		return atoms;
	}
	
	/**
	 * teste si la base est vide
	 * @return vrai si la base est vide
	 */
	public boolean isEmpty() 
	{
		return atoms.isEmpty();
	}
	
	/**
	 * retourne le nombre de faits dans la base
	 * @return le nombre de faits dans la base
	 */
	public int size()
	{
		return atoms.size();
	}	
	
	/**
	 * Teste l'appartenance d'un atome a la base de faits
	 * @param a l'atome dont on teste l'existence
	 * @return vrai si l'atome existe deja, faux sinon
	 */
	public boolean belongsAtom(Atom a)
	{
		for(int i=0;i<atoms.size();i++)
			if(atoms.get(i).equals(a))
					return true;
		return false;
	}
	
	
	/**
	 * Retourne une description de la base de faits
	 * @return description de la base de faits
	 * @see java.lang.Object#toString()
	 */
	public String toString()
	{
		String s ="";
		if (atoms.size()==0) s="La base est vide !";
		else {
			s = "La base contient "+atoms.size()+ " faits : \n";
			for(int i=0;i<atoms.size()-1;i++)
				s+=	atoms.get(i)+" ; ";	
			s+= atoms.get(atoms.size()-1)+"\n";
			}
		return s;
	}
}
	