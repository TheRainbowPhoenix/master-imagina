public class CompteAvecReduction extends Compte{

    protected double tauxReduction;

    public CompteAvecReduction(Client c){
        super(c);
        tauxReduction = 1.0 - 0.40;
    }

    @Override
    public double prixLocation(Produit p){
        return super.prixLocation(p) * tauxReduction;
    }
}
