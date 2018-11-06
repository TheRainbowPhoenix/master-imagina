package Decorateur;

public class ProduitSolde extends Produit {

    protected double tauxSolde;

    public ProduitSolde(String nom, double prix){
        super(nom, prix);
        tauxSolde = 1.0 - 0.20;
    }

    @Override
    public double prix(){
        return super.prix() * tauxSolde;
    }
}
