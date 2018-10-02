package pack;

public class ViderBidon extends Commande {

	private Bidon bid;
	
	public ViderBidon(Bidon b) {
		this.bid = b;
	}
	
	@Override
	public void execute() {
		setVolDeplace(bid.getVolume());
		bid.vider();

	}

	@Override
	public void cancel() {
		bid.setVolume(getVolDeplace());
	}

}
