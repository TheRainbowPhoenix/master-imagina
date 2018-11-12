public class Produit {

    protected String nom;
    protected double prix;

    public Produit(String nom, double prix){
        this.nom = nom;
        this.prix = prix;
    }

    public String nom(){
        return nom;
    }

    public double prix(){
        return prix;
    }
}
