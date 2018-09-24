package csp_etud;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * 
 * Permet la manipulation aisée des assignations. Une assignation (Assignment) est un ensemble de couples (variable, valeur associée).
 * Les noms des variables sont des chaînes de caractères (String)
 * Leurs valeurs sont des objects quelconques (Object).
 * 
 * Toute variable de l'assignation doit avoir une valeur associée !
 *
 */

/* (non-Javadoc)
 * Pour manipuler une assignation (Assignment) :
 * 	- new Assignment() : pour créer une assignation vide
 *  - clear() : pour remettre à vide l'assignation
 *  - isEmpty() : pour tester si l'assignation est vide
 * 	- put(var,valeur) : pour compléter l'assignation avec le couple (var, valeur) si var n'est pas déjà présente
 *  					sinon pour changer la valeur associée à var 
 *  - remove(var) : pour supprimer de l'assignation la variable var
 *	- getVars() : pour récupérer la liste des variables de l'assignation
 *	- get(var) : pour récupérer la valeur d'une variable de l'assignation
 *  - clone() : pour récupérer une copie de l'assignation
 */
@SuppressWarnings("serial")
public class Assignment extends HashMap<String,Object> {

	/**
	 * Construit une assignation vide
	 */
	public Assignment() {
		super();
	}
	
	/**
	 * Retourne une copie "superficielle" de l'assignation (copie de la hashmap, mais pas de recopie des variables et des valeurs)
	 * Cette copie suffit si on ne modifie pas le nom d'une variable ni un objet valeur lui-même 
	 * 
	 * @return la copie de l'assignation 
	 */
	public Assignment clone() {
		return (Assignment)super.clone();
	}
	
	/**
	 * Retourne la liste des variables de l'assignation (c'est à dire toutes les variables assignées)
	 * 
	 * @return la liste des variables
	 */
	public ArrayList<String> getVars(){
		return new ArrayList<String>(keySet());
	}
}
