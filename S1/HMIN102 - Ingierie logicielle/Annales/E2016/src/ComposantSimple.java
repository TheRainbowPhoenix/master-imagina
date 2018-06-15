
public abstract class ComposantSimple extends ComposantOrdi{
		
	protected String nom;	
	
	public ComposantSimple(double TVA, double prixAchat, double marge, String nom) {
		this.TVA = TVA;
		this.prixAchat = prixAchat;
		this.marge = marge;
		this.nom = nom;
	}
	
	public ComposantSimple(double TVA, double prixAchat, double marge) {
		this.TVA = TVA;
		this.prixAchat = prixAchat;
		this.marge = marge;
	}

	protected double prixHT(){
		return this.prixAchat + (this.prixAchat * this.marge / 100);
	}
	
	public String toString(){
		return this.nom;
	}
	
	/*protected boolean equiv(ComposantOrdi composant , String critère){
		if(critère.equals("nom")) {
			return (this.nom.equals(composant.toString()));
		} else if(critère.equals("prixAchat")) {
			return (this.prixAchat == composant.prixAchat);
		} else if(critère.equals("prixTTC")) {
			return (this.prixTTC() == composant.prixTTC());
		} else if(critère.equals("prixHT")) {
			return (this.prixHT() == composant.prixHT());
		} else if(critère.equals("marge")) {
			return (this.marge == composant.marge);
		} else if(critère.equals("TVA")) {
			return (this.TVA == composant.TVA);
		} else return false;
	}*/
}
