package pack;

/**
 * 
 * @author ahmad
 *
 */
public class RemplirBidon extends Commande {

	private Bidon bid;
	
	public RemplirBidon(Bidon bi){
		this.bid = bi;
	}
	
	@Override
	public void execute() {
		setVolDeplace(bid.getVolumeMax() - bid.getVolume());
		bid.remplir();
	}

	@Override
	public void cancel() {
		bid.setVolume(bid.getVolume()-getVolDeplace());
	}

}
