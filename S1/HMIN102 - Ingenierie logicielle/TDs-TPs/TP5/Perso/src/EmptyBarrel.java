/**
 * Created by ecrvn on 10/27/2016.
 */
public class EmptyBarrel extends Command{

    public EmptyBarrel(Barrel b){
        this.moved = 0;
        this.barrel = b;
    }

    @Override
    public void execute(){
        this.moved = this.barrel.empty();
    }

    @Override
    public void cancel(){
        this.barrel.setVolume(this.barrel.getVolume() + this.moved);
        this.moved = 0;
    }
}
