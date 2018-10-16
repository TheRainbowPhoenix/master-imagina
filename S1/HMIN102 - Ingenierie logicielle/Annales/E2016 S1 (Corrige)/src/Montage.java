import java.util.ArrayList;

public class Montage extends ComposantOrdi {
	protected ArrayList<ComposantOrdi> composants;
	
	public Montage() {
		composants = new ArrayList<ComposantOrdi>();
	}
	
	public Montage add(ComposantOrdi composant){
		this.composants.add(composant);
		return this;
	}
	
	protected double prixHT(){
		double total = 0;
		for(ComposantOrdi comp : this.composants)
			total += comp.prixHT();
		return total;
	}
	
	protected double prixTTC() {
		double total = 0;
		for(ComposantOrdi comp : this.composants)
			total += comp.prixTTC();
		return total;	
	}
	
	public String toString(){
		String str = new String();
		str += "Montage:\n";
		for(ComposantOrdi composant : this.composants)
			str += composant.toString() + ": " + composant.prixTTC() + "\n";
		str += this.prixTTC();
		return str;
	}

	protected boolean equiv(Montage montage, String critère) {
		System.out.println("Montage");
		return false;
	}
}