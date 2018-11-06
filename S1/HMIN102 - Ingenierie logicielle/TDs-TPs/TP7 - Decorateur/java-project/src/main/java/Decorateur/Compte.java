package Decorateur;

public class Compte {

    protected Client client;

    public Compte(){
        client = new Client("");
    }

    public Compte(Client c){
        this.client = c;
    }

    public Client client(){
        return client;
    }

    public double prixLocation(Produit p){
        return p.prix();
    }
}
