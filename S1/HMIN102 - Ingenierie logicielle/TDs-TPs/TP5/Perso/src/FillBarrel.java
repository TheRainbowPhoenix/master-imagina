/**
 * Created by ecrvn on 10/27/2016.
 */
public class FillBarrel extends Command{

    public FillBarrel(Barrel b){
        this.moved = 0;
this.barrel = b;
    }

    @Override
    public void execute() {
        this.moved = this.barrel.fill();
    }

    @Override
    public void cancel(){
        this.barrel.setVolume(this.barrel.getVolume() - this.moved);
        this.moved = 0;
    }
}
