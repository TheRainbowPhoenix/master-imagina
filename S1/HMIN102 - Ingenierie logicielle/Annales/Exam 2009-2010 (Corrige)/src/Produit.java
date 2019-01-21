public abstract class Produit {

	protected String nom;
	protected double prixAchat;
	protected double TVA;

	Produit(String nom, double prixAchat, double TVA) {
		this.nom = nom;
		this.prixAchat = prixAchat;
		this.TVA = TVA;
	}

	public double prixHT() {
		return prixAchat;
	}

	public double prixTTC() {
		return this.prixHT() + (this.prixHT() * this.TVA / 100);
	}
} 
