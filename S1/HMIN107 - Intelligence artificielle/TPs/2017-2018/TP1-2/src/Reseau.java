import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

/* (non-Javadoc)
 *  Choix de codage :
 *		Variable = String
 *      Valeur = Object
 *		un domaine = ArrayList<Object>
 *
 *  un réseau de contrainte (X,D,C) est représenté par :
 * 		- X les variables et D les domaines sont représentés par un ensemble de 
 *		couples (variable,domaine) utilisant une HashMap<String,ArrayList<Object>>
 *		- C les contraintes est une simple liste de contraintes : ArrayList<Constraint>
 *
 *  On pourra, pour optimiser la structure d'un réseau de contraintes, ajouter
 * une HashMap (variable, liste de contraintes) qui permet d'associer à
 * chaque variable la liste des contraintes qui portent sur elle.
 *
*/
/**
 * Structure de manipulation d'un réseau de contraintes (X,D,C)
 *
 */
public class Reseau {

	private HashMap<String,ArrayList<Object>> varDom;
	private ArrayList<Constraint> constraints;

	/**
	 * Construit un réseau de contraintes sans variable (ni contrainte)
	 */
	public Reseau() {
		varDom = new HashMap<String,ArrayList<Object>>();
		constraints = new ArrayList<Constraint>();
	}

	/**
	 * Construit un réseau de contraintes à partir d'une représentation textuelle au 
	 * format :
	 * <p> nombre de variables
	 * <p> nom de la variable 1 ; val1 ; val2 ; ...
	 * <p> nom de la variable 2 ; val1 ; val2 ; ...
	 * <p> ...
	 * <p> nombre de contraintes
	 * <p> type de la contrainte 1   (pour l'instant uniquement 'ext')
	 * <p> nom de la 1ière variable de la contrainte 1 ; nom de la 2nde var de la contrainte 1 ; ...
	 * <p> nombre de tuples de valeurs pour cette contrainte 
	 * <p> val1 ; val2 ; ...
	 * <p> val1 ; val2 ; ... 
	 * <p> type de la contrainte 2   (pour l'instant uniquement 'ext')
	 * <p> nom de la 1ière variable de la contrainte 2 ; nom de la 2nde var de la contrainte 2 ; ...
	 * <p> nombre de tuples de valeurs pour cette contrainte 
	 * <p> val1 ; val2 ; ...
	 * <p> val1 ; val2 ; ... 
	 * <p> ...
	 *
	 * @param in le buffer de lecture de la représentation textuelle de la contrainte
	 * @throws Exception en cas de problème dans l'analyse textuelle
	 */
	public Reseau(BufferedReader in) throws Exception {
		varDom = new HashMap<String,ArrayList<Object>>();
		constraints = new ArrayList<Constraint>();
		// Les variables et domaines
		int nbVariables = Integer.parseInt(in.readLine());		// le nombre de variables
		for(int i=1;i<=nbVariables;i++) {
			String[] varDeclaration = in.readLine().split(";"); // Var;Val1;Val2;Val3;...
			ArrayList<Object> dom = new ArrayList<Object>();
			varDom.put(varDeclaration[0], dom);
			for(int j=1;j<varDeclaration.length;j++) dom.add(varDeclaration[j]);
		}
		// Les contraintes
		int nbConstraints = Integer.parseInt(in.readLine());	// le nombre de contraintes
		for(int k=1;k<=nbConstraints;k++) {
			Constraint c = null;
			String type = in.readLine().trim();					// le type de la contrainte 
			if (type.equals("ext")) c = new ConstraintExt(in);
			else if(type.equals("dif")) c = new ConstraintDif(in);
			else if(type.equals("eq")) c = new ConstraintEq(in);
				//else if(type.equals("AUTRE TYPE")) c = new ConstraintTYPE(in)
			else System.err.println("Type contrainte inconnu");
			constraints.add(c);
		}
	}

	/**
	 * Ajoute une nouvelle variable dans le réseau avec un domaine vide.
	 *
	 * @param var la variable à ajouter
	 */
	public void addVariable(String var) {
		if(varDom.get(var)==null) varDom.put(var, new ArrayList<Object>());
		else System.err.println("Variable " + var + " deja existante");
	}

	/**
	 * Ajoute une nouvelle valeur au domaine d'une variable du réseau
	 *
	 * @param var la variable à laquelle il faut ajouter la valeur
	 * @param val la valeur à ajouter
	 */
	public void addValue(String var, Object val) {
		if(varDom.get(var)==null) System.err.println("Variable " + var + " non existante");
		else {
			ArrayList<Object> dom = varDom.get(var);
			if (!dom.contains(val))
				dom.add(val);
			else
				System.err.println("La valeur " + val + " est déjà dans le domaine de la variable " + var);
		}
	}

	/**
	 * Ajoute une contrainte dans le réseau. Les variables de la
	 * contrainte doivent avoir été précédemment ajoutées au réseau.
	 *
	 * @param c la contrainte à ajouter
	 */
	public void addConstraint(Constraint c) {
		// Attention !! On ne vérifie pas que les valeurs des contraintes sont "compatibles" avec les domaines
		if(!varDom.keySet().containsAll(c.getVars()))
			System.err.println("La contrainte " + c.getName() + " contient des variables (" + c.getVars()
					+ ") non déclarées dans le CSP dont les variables sont " + varDom.keySet());
		else constraints.add(c);
	}

	/**
	 * Retourne le nombre de variables du réseau
	 *
	 * @return le nombre de variables
	 */
	public int getVarNumber() {
		return varDom.size();
	}

	/**
	 * Retourne la taille du domaine d'une variable du réseau.
	 *
	 * @param var la variable dont on veut connaître la taille de son domaine
	 * @return le nombre de valeurs de Dom(var)
	 */
	public int getDomSize(String var) {
		return varDom.get(var).size();
	}

	/**
	 * Retourne le nombre de contraintes du réseau
	 *
	 * @return le nombre de contraintes
	 */
	public int getConstraintNumber(){
		return constraints.size();
	}

	/**
	 * Retourne la liste des variables du réseau
	 *
	 * @return la liste des variables
	 */
	public ArrayList<String> getVars() {
		return new ArrayList<String>(varDom.keySet());
	}

	/**
	 * Retourne le domaine d'une variable
	 *
	 * @param var la variable dont on veut le domaine
	 * @return la liste des valeurs du domaine d'une variable
	 */
	public ArrayList<Object> getDom(String var) {
		return varDom.get(var);
	}

//	public HashMap<String,ArrayList<Object>> getDom() {
//		return varDom;
//	}

	/**
	 * Retourne la liste des contraintes du réseau
	 *
	 * @return la liste des contraintes
	 */
	public ArrayList<Constraint> getConstraints() {
		return constraints;
	}

	/**
	 * Retourne la liste des contraintes du réseau portant sur une
	 * variable donnée
	 *
	 * @param var la variable dont on veut connaitre ses contraintes
	 * @return la liste des contraintes contraignant var
	 */
	public ArrayList<Constraint> getConstraints(String var) {
		ArrayList<Constraint> selected = new ArrayList<Constraint>();
		for(Constraint c : constraints)
			if(c.getVars().contains(var)) selected.add(c);
		return selected;
	}

//	/**
//	 * 
//	 * @param var une variable du réseau de contraintes
//	 * @param vars un ensemble de variables du réseau de contraintes
//	 * @return l'ensemble de contraintes qui portent sur var et dont la liste des variables est incluse dans vars
//	 */
//	public ArrayList<Constraint> getConstraintsToCheck(String var, ArrayList<String> vars) {
//		ArrayList<Constraint> selected = new ArrayList<Constraint>();
//		for(Constraint c : getConstraints(var)) {								
//			if(vars.containsAll((c.getVariables()))) selected.add(c);
//		}
//		return selected;
//	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "Var et Dom : " + varDom + "\nConstraints :" + constraints;
	}

	public static void main(String[] args) throws Exception{
		String filename;
		BufferedReader lectureF;
		Reseau monCSP;
		CSP csp;
		Assignation firstSolution;
		ArrayList<Assignation> allSolutions;
		
		/*__________PROBLEME DES 4 REINES__________*/
		System.out.println("Problème des 4 reines");
		filename = "4reines.txt";
		//System.out.println("Chargement du fichier : " + new java.io.File(".").getCanonicalPath() + "/" + filename);
		lectureF = new BufferedReader(new FileReader (filename));
		monCSP = new Reseau(lectureF);
		csp = new CSP(monCSP);
		//System.out.println("Mon réseau de contraintes :\n" + monCSP);
		//Calcul de la première solution
		firstSolution = csp.searchSolution();
		System.out.println("Solution trouvée : " + firstSolution);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
		allSolutions = csp.searchAllSolutions();
		System.out.println("Solutions trouvées : " + allSolutions);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
		System.out.println("");

		
		/*__________PROBLEME DE COLORATION__________*/
		System.out.println("Problème de coloration");
		filename = "Coloration.txt";
		//System.out.println("Chargement du fichier : " + new java.io.File(".").getCanonicalPath() + "/" + filename);
		lectureF = new BufferedReader(new FileReader (filename));
		monCSP = new Reseau(lectureF);
		csp = new CSP(monCSP);
		//System.out.println("Mon réseau de contraintes :\n" + monCSP);
		//Calcul de la première solution
		firstSolution = csp.searchSolution();
		System.out.println("Solution trouvée : " + firstSolution);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
		allSolutions = csp.searchAllSolutions();
		System.out.println("Solutions trouvées : " + allSolutions);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
		System.out.println("");
		
		/*__________PROBLEME D'HOMORPHISME__________*/
		System.out.println("Problème d'homomorphisme");
		filename = "homomorphisme.txt";
		//System.out.println("Chargement du fichier : " + new java.io.File(".").getCanonicalPath() + "/" + filename);
		lectureF = new BufferedReader(new FileReader (filename));
		monCSP = new Reseau(lectureF);
		csp = new CSP(monCSP);
		//System.out.println("Mon réseau de contraintes :\n" + monCSP);
		//Calcul de la première solution
		firstSolution = csp.searchSolution();
		System.out.println("Solution trouvée : " + firstSolution);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
		allSolutions = csp.searchAllSolutions();
		System.out.println("Nombre de solutions trouvées : " + allSolutions.size());
		System.out.println("");
		
		/*__________PROBLEME DES 8 REINES__________*/
		System.out.println("Problème des 8 reines");
		filename = "8reinesExt.txt";
		//System.out.println("Chargement du fichier : " + new java.io.File(".").getCanonicalPath() + "/" + filename);
		lectureF = new BufferedReader(new FileReader (filename));
		monCSP = new Reseau(lectureF);
		csp = new CSP(monCSP);
		//System.out.println("Mon réseau de contraintes :\n" + monCSP);
		//Calcul de la première solution
		firstSolution = csp.searchSolution();
		System.out.println("Solution trouvée : " + firstSolution);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
		allSolutions = csp.searchAllSolutions();
		System.out.println("Nombre de solutions trouvées : " + allSolutions.size());
		System.out.println("");
		
		/*__________PROBLEME DU ZEBRE__________*/
		System.out.println("Problème du zèbre");
		filename = "zebre.txt";
		//System.out.println("Chargement du fichier : " + new java.io.File(".").getCanonicalPath() + "/" + filename);
		lectureF = new BufferedReader(new FileReader (filename));
		monCSP = new Reseau(lectureF);
		csp = new CSP(monCSP);
		//System.out.println("Mon réseau de contraintes :\n" + monCSP);
		//Calcul de la première solution
		firstSolution = csp.searchSolution();
		System.out.println("Solution trouvée : " + firstSolution);
		System.out.println("Nombre de noeuds explorés : " + csp.cptr);
	}
}

