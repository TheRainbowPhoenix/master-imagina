
public abstract class ComposantOrdi {
		
	protected double TVA;
	
	protected double prixAchat;
	
	protected double marge;
			
	protected abstract double prixHT();
	
	protected double prixTTC() {
		return this.prixHT() + (this.prixHT() * this.TVA / 100);
	}
	
	protected boolean equiv(ComposantOrdi composant , String critère){
		System.out.println("Composant Ordi");
		return false;
	}
}
