package pack;

/**
 * 
 * @author ahmad
 *
 */
public abstract class Commande implements ICommande {
	
	/**
	 * volume deplace pendant l'operation
	 */
	private int volDeplace;

	public int getVolDeplace() {
		return volDeplace;
	}

	public void setVolDeplace(int volDeplace) {
		this.volDeplace = volDeplace;
	}
}
