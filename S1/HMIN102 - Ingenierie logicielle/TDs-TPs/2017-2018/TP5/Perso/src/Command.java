/**
 * Created by ecrvn on 10/27/2016.
 */
public abstract class Command implements ICommand{
    protected int moved;
    protected Barrel barrel;

    public int getMoved(){
        return  this.moved;
    }

    public void setMoved(int volume){
        this.moved = volume;
    }
}
