import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * Permet la manipulation aisée des assignations, i.e. des ensembles de variables avec leurs valeurs associées.
 * Les noms des variables sont de simples chaînes de caractères (String)
 * Leurs valeurs sont des objects quelconques (Object).
 *
 * Toute variable de l'assignation doit avoir une valeur associée !
 *
 */

/* (non-Javadoc)
 * Pour manipuler une Assignation :
 * 	- new assignation() : pour créer une assignation vide
 *  - clear() : pour remettre à vide une assignation
 *  - isEmpty() : pour tester si une assignation est vide
 * 	- put(var,valeur) : pour compléter l'assignation avec une nouvelle variable
 *                       assignée à une valeur donnée (ou modifier la valeur d'une
 *                       variable déjà assignée)
 *  - remove() : pour supprimer de l'assignation une variable assignée
 *	- getVars() : pour récupérer la liste des variables assignées
 *	- get(var) : pour récupérer la valeur d'une variable de l'assignation
 *  - clone() : pour récupérer une copie d'une assignation
 */
@SuppressWarnings("serial")
public class Assignation extends HashMap<String,Object> {

	/**
	 * Construit une assignation vide
	 */
	public Assignation() {
		super();
	}

	/**
	 * Retourne une copie "superficielle" de l'assignation (les variables et valeurs ne sont pas copiées)
	 *
	 * @return la copie de l'assignation 
	 */
	public Assignation clone() {
		return (Assignation)super.clone();
	}

	/**
	 * Retourne la liste des variables de l'assignation (i.e. les variables assignées)
	 *
	 * @return la liste des variables
	 */
	public ArrayList<String> getVars(){
		return new ArrayList<String>(keySet());
	}
}
