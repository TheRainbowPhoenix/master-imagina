import java.util.ArrayList;

/***
 * Represents an instance of CSP, aggregating a network of constraints and keeping track of the assignment(s).
 * Calculates one or all solutions.
 */
public class CSP {

    public CstNetwork CstNetwork;                // le réseau à résoudre
    private ArrayList<Assignment> solutions;    // les solutions du réseau
    private Assignment assignment;                // l'assignment courante
    public int cptr;                            // le compteur de noeuds explorés

    /**
     * Crée un problème de résolution de contraintes pour un réseau donné
     *
     * @param r le réseau de contraintes à résoudre
     */
    public CSP(CstNetwork r) {
        CstNetwork = r;
        solutions = new ArrayList<Assignment>();
        assignment = new Assignment();
    }

    /**
     * Cherche une solution au réseau de contraintes
     *
     * @return une assignment solution du réseau ou null si pas de solution
     */
    public Assignment searchSolution() {
        cptr = 0;
        assignment.clear();
        Assignment sol = backtrack();
        return sol;
    }

    /**
     * Calcule toute les solutions au réseau de contraintes
     *
     * @return la liste des assignments solution
     */
    public ArrayList<Assignment> searchAllSolutions() {
        cptr = 0;
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
    private Assignment backtrack() {
        cptr++;
        if (this.assignment.size() == this.CstNetwork.getVarNumber()) {
            return assignment;
        }

        String x = chooseVar();
        ArrayList<Object> domain = sort(CstNetwork.getDom(x));
        for (int i = 0; i < domain.size(); i++) {
            assignment.put(x, domain.get(i));
            if (this.consistent(x)) {
                Assignment b = backtrack();
                if (b != null)
                    return this.assignment;
                this.assignment.remove(x);
            }
        }
        return null;
    }

    /**
     * Exécute l'algorithme de backtrack à la recherche de toutes les solutions
     * à partir de l'assignment courante en stockant solutions rencontrées.
     */
    private void backtrackAll() {
        cptr++;
        if (this.assignment.size() == this.CstNetwork.getVarNumber()) {
            this.solutions.add(this.assignment.clone());
            return;
        }
        String x = chooseVar();
        ArrayList<Object> domain = sort(CstNetwork.getDom(x));
        for (int i = 0; i < domain.size(); i++) {
            assignment.put(x, domain.get(i));
            if (this.consistent(x)) {
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
    private String chooseVar() {
        for (String var : CstNetwork.getVars()) {
            if (!(assignment.containsKey(var))) {
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
        ArrayList<Constraint> constraints = CstNetwork.getConstraints(lastAssignedVar);
        for (int i = 0; i < constraints.size(); i++) {
            if (constraints.get(i).violation(assignment))
                return false;
        }
        return true;
    }
}