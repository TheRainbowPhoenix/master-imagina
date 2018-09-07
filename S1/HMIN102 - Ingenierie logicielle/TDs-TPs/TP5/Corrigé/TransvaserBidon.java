package pack;

/**
 * 
 * @author ahmad
 *
 */
public class TransvaserBidon extends Commande {

	
	private Bidon source , destination;
	
	public TransvaserBidon(Bidon src,Bidon dst) {
		this.source = src;
		this.destination = dst;
	}
	
	@Override
	public void execute() {
		// le vol deplace est retoune par la methode transvaser
		setVolDeplace(source.transvaser(destination));
	}

	@Override
	public void cancel() {
		source.setVolume(source.getVolume() + getVolDeplace());
		destination.setVolume(destination.getVolume() - getVolDeplace());
	}

}
