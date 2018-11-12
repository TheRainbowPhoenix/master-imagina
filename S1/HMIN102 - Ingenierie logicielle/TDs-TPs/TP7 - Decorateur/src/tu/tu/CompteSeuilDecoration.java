public class CompteSeuilDecoration extends CompteDecoration {

    protected int seuil;
    protected int nbLocation;

    public CompteSeuilDecoration(Compte c){
        super(c);
        seuil = 2;
        nbLocation = 0;
    }

    @Override
    public double prixLocation(Produit p){

        if(nbLocation < seuil){
            nbLocation++;
            return super.prixLocation(p);
        }
        else {
            nbLocation = 0;
            return 0;
        }
    }
}
