import java.util.ArrayList;
import java.util.HashMap;

/**
 * Represents assignments as a list of variables and their assigned value.
 * Variable names are Strings.
 * Their values are Objects.
 * Every variable must have an assigned value.
 */

/* (non-Javadoc)
 * Pour manipuler une Assignment :
 * 	- new Assignment() : pour créer une Assignment vide
 *  - clear() : pour remettre à vide une Assignment
 *  - isEmpty() : pour tester si une Assignment est vide
 * 	- put(var,valeur) : pour compléter l'Assignment avec une nouvelle variable
 *                       assignée à une valeur donnée (ou modifier la valeur d'une
 *                       variable déjà assignée)
 *  - remove() : pour supprimer de l'Assignment une variable assignée
 *	- getVars() : pour récupérer la liste des variables assignées
 *	- get(var) : pour récupérer la valeur d'une variable de l'Assignment
 *  - clone() : pour récupérer une copie d'une Assignment
 */
@SuppressWarnings("serial")
public class Assignment extends HashMap<String,Object> {

    /**
     * Construit une Assignment vide
     */
    public Assignment(){
        super();
    }

    /**
     * Retourne une copie "superficielle" de l'Assignment (les variables et valeurs ne sont pas copiées)
     *
     * @return la copie de l'Assignment 
     */
    public Assignment clone(){
        return (Assignment)super.clone();
    }

    /**
     * Retourne la liste des variables de l'Assignment (i.e. les variables assignées)
     *
     * @return la liste des variables
     */
    public ArrayList<String> getVars(){
        return new ArrayList<String>(keySet());
    }
}
