public class Compte {
	
	protected static int numCompte;
	protected Client client;
	protected String dateCreation;
	protected double montant;

	Compte(Client client, String dateCreation, double montant){
		numCompte++;
		this.client = client;
		this.dateCreation = dateCreation;
		this.montant = montant;
	}

	double prixFinalLocation(float prixCourant){
		
	}
}