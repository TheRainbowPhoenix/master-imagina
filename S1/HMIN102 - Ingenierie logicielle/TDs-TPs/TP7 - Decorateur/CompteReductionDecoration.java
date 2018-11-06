public class CompteReductionDecoration extends CompteDecoration{

    protected double tauxReduction;

    public CompteReductionDecoration(Compte c){
        super(c);
        tauxReduction = 1.0 - 0.40;
    }

    @Override
    public double prixLocation(Produit p){
        return super.prixLocation(p) * tauxReduction;
    }
}
