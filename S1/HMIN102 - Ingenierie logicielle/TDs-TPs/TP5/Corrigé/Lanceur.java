package pack;

import java.util.Stack;

/**
 * 
 * @author Ahmad BEDJA BOANA
 * 
 * possible amelioration lier les bidons aux lanceur
 *
 */
public class Lanceur {
	
	private Stack<ICommande> history;
	
	public Lanceur(){
		this.history = new Stack<ICommande>();
	}
	
	/**
	 * execute la commande c et la sauve dans l'historique
	 * @param c
	 */
	public void pushCommand(ICommande c){
		c.execute();
		history.push(c);
	}
	
	/**
	 * annule la commande la plus recente et la retire de l'historique
	 */
	public void popLastCommand(){
		history.lastElement().cancel();
		history.pop();
	}
	
	/**
	 * 
	 * fction de debug pour verifier l'etat de bidons
	 * 
	 * @param b1
	 * @param b2
	 * @param b3
	 * @return
	 */
	public static String etatBidon(Bidon b1,Bidon b2,Bidon b3){
		return b1.getVolume() + " "+b2.getVolume()+" "+b3.getVolume();
	}
	public static void main(String[] args) {

		Lanceur lc = new Lanceur();
		
		Bidon b3 = new Bidon(3);
		Bidon b4 = new Bidon(4);
		Bidon b6 = new Bidon(6);

		lc.pushCommand(new RemplirBidon(b3));
		//lc.pushCommand(new RemplirBidon(b4));
		lc.pushCommand(new RemplirBidon(b6));
		System.out.println(etatBidon(b3, b4, b6));
		
		lc.pushCommand(new TransvaserBidon(b3,b4));
		System.out.println(etatBidon(b3, b4, b6));
		
		lc.pushCommand(new TransvaserBidon(b6,b3));
		System.out.println(etatBidon(b3, b4, b6));
		
		lc.pushCommand(new TransvaserBidon(b4,b6));
		System.out.println(etatBidon(b3, b4, b6));
		
		lc.pushCommand(new ViderBidon(b6));
		System.out.println(etatBidon(b3, b4, b6));
		
		System.out.println("maintenant on annule");
		while(!lc.history.empty()){ 
			lc.popLastCommand();
			System.out.println(etatBidon(b3, b4, b6));
		}
		
	}
}
