import java.util.ArrayList;

/**
 * Permet de résoudre un problème de contrainte par Backtrack : 
 * 	Calcul d'une solution, 
 * 	Calcul de toutes les solutions
 *
 */
public class CSP {

    private Reseau reseau;						// le réseau à résoudre
    private ArrayList<Assignation> solutions; 	// les solutions du réseau
    private Assignation assignment;				// l'assignment courante
    public int cptr;							// le compteur de noeuds explorés

    /**
     * Crée un problème de résolution de contraintes pour un réseau donné
     *
     * @param r le réseau de contraintes à résoudre
     */
    public CSP(Reseau r) {
        reseau = r;
        solutions = new ArrayList<Assignation>();
        assignment = new Assignation();
    }

    /**
     * Cherche une solution au réseau de contraintes
     *
     * @return une assignment solution du réseau ou null si pas de solution
     */
    public Assignation searchSolution() {
        cptr=0;
        assignment.clear();
        Assignation sol = backtrack();
        return sol;
    }

    /**
     * Calcule toute les solutions au réseau de contraintes
     *
     * @return la liste des assignments solution
     *
     */
    public ArrayList<Assignation> searchAllSolutions(){
        cptr=0;
        solutions.clear();
        assignment.clear();
        this.backtrackAll();
        return this.solutions;
    }

    /**
     * Exécute l'algorithme de backtrack à la recherche d'une solution à partir de l'assignment courante
     *
     * @return la prochaine solution ou null si pas de nouvelle solution
     */
    private Assignation backtrack() {
        cptr++;
        if(this.assignment.size() == this.reseau.getVarNumber()){
            return assignment;
        }

        String x = chooseVar();
        ArrayList<Object> domain = sort(reseau.getDom(x));
        for(int i = 0 ; i < domain.size() ; i++) {
            assignment.put(x, domain.get(i));
            if(this.consistent(x)) {
                Assignation b = backtrack();
                if(b != null)
                    return this.assignment;
                this.assignment.remove(x);
            }
        }
        return null;
    }

    /**
     * Exécute l'algorithme de backtrack à la recherche de toutes les solutions
     * à partir de l'assignment courante en stockant solutions rencontrées.
     *
     */
    private void backtrackAll() {
        cptr++;
        if(this.assignment.size() == this.reseau.getVarNumber()){
            this.solutions.add(this.assignment.clone());
            return;
        }

        String x = chooseVar();
        ArrayList<Object> domain = sort(reseau.getDom(x));
        for(int i = 0 ; i < domain.size() ; i++){
            assignment.put(x, domain.get(i));
            if(this.consistent(x)) {
                this.backtrackAll();
            }
            this.assignment.remove(x);
        }
        return;
    }

    /**
     * Retourne la prochaine variable à assigner
     *
     * @return une variable non encore assignée
     */
    private String chooseVar(){
        for(String var : reseau.getVars()){
            if(!(assignment.containsKey(var))){
                return var;
            }
        }
        return null;
    }

    /**
     * Fixe un ordre de prise en compte des valeurs d'un domain
     *
     * @param values une liste de valeurs
     * @return une liste de valeurs
     */
    private ArrayList<Object> sort(ArrayList<Object> values) {
        return values;
    }

    /**
     * Teste si l'assignment courante est consistante, c'est à dire qu'elle
     * ne viole aucune contrainte.
     *
     * @param lastAssignedVar la variable que l'on vient d'assigner à cette étape.
     * @return vrai ssi l'assignment courante ne viole aucune contrainte
     */
    private boolean consistent(String lastAssignedVar) {
        ArrayList<Constraint> constraints = reseau.getConstraints(lastAssignedVar);
        for(int i = 0 ; i < constraints.size() ; i++) {
            if(constraints.get(i).violation(assignment))
                return false;
        }
        return true;
    }
}