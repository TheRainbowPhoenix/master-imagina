/**
 * Created by ecrvn on 10/27/2016.
 */
public class PourBarrel extends Command{
    private Barrel destinationBarrel;

    public PourBarrel(Barrel source , Barrel destination){
        this.moved = 0;
        this.barrel = source;
        this.destinationBarrel = destination;
    }

    @Override
    public void execute() {
        this.moved = this.barrel.pourInto(destinationBarrel);
    }

    @Override
    public void cancel(){
        this.barrel.setVolume(this.barrel.getVolume() + this.moved);
        this.destinationBarrel.setVolume(this.destinationBarrel.getVolume() - this.moved);
        this.moved = 0;
    }
}
