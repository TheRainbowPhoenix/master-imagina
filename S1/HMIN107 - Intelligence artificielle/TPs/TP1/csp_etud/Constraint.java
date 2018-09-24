package csp_etud;

import java.io.BufferedReader;
import java.util.ArrayList;

/**
 * Classe abstraite pour une contrainte. Cette classe a vocation
 * à être spécialisée pour chaque type de contrainte en redéfinissant :
 * un constructeur spécifique, un constructeur permettant de lire la contrainte
 * à partir d'une représentation textuelle, une méthode de test de la violation de la contrainte,
 * la méthode de représentation textuelle de la contrainte.
 * 
 */
public abstract class Constraint {
	
	protected static int num = 0; 			// pour générer un nom différent pour chaque contrainte
	protected String name; 					// nom de la contrainte
	protected ArrayList<String> varList;	// portée de la contrainte : la liste des différentes variables
											//                           mises en jeu par la contrainte
	
	/**
	 * Construit une contrainte portant sur une liste de variables.
	 * Les variables doivent être différentes. L'arité de la contrainte
	 * est la taille de la liste.
	 * Le nom de la contrainte est généré automatiquement
	 * 
	 * @param vars les variables sur lesquelles la contrainte porte
	 * 
	 */
	public Constraint(ArrayList<String> vars) {
		num++;
		this.name = "C"+num;
		varList = vars; 
	}
	
	/**
	 * Construit une contrainte portant sur une liste de variables.
	 * Les variables doivent être différentes. L'arité de la contrainte
	 * est la taille de la liste.
	 * Le nom de la contrainte est passé en paramètre
	 * 
	 * @param vars les variables de la contrainte 
	 * @param name le nom de la contrainte
	 */
	public Constraint(ArrayList<String> vars, String name) {
		num++;
		this.name = name;
		varList = vars; 
	}
	
	/**
	 * Construit une contrainte à partir d'une représentation textuelle de
	 * la contrainte. La liste de variables est donnée sous la forme : Var1;Var2;...
	 * Les variables doivent être différentes. L'arité de la contrainte
	 * correspond au nombre de variables lues.
	 * Le nom de la contrainte est généré automatiquement.
	 * 
	 * @param in le buffer de lecture de la représentation textuelle de la contrainte
	 * @throws Exception en cas d'erreur de format 
	 */
	public Constraint(BufferedReader in) throws Exception {
		num++;
		this.name = "C"+num;
		varList = new ArrayList<String>();
		for (String v : in.readLine().split(";")) varList.add(v);	// Var1;Var2;...;Var(arity)
	}
	
	/**
	 * Retourne l'arité de la contrainte
	 * 
	 * @return l'arité
	 */
	public int getArity() {
		return varList.size();
	}
	
	/**
	 * Retourne le nom de la contrainte
	 * 
	 * @return son nom
	 */
	public String getName() {
		return name;
	}
	
	
	/**
	 * Retourne la portée de la contrainte, c'est à dire la liste de ses variables 
	 * 
	 * @return la liste des variables de la contrainte
	 */
	public ArrayList<String> getVars() {
		return varList;
	}

	/**
	 * 
	 * Teste si une assignation viole la contrainte. 
	 * Doit être implantée dans la classe concrète. 
	 * @param a l'assignation à tester
	 * @return vrai ssi l'assignation viole la contrainte 
	 */
	public abstract boolean violation(Assignment a);
	
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "\n\t"+ name + " " + varList; 
	}

}
