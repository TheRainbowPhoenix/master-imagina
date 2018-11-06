package Decorateur;

public abstract class CompteDecoration extends Compte{

    protected Compte compte;

    public CompteDecoration(Compte c){
        compte = c;
    }

    @Override
    public double prixLocation(Produit p){
        return compte.prixLocation(p);
    }
}
