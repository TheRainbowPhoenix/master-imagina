/**
 * Created by ecrvn on 10/27/2016.
 */
public class Barrel {
    private int maxVolume;
    private int volume;

    public Barrel(int maxv){
        this.volume = 0;
        this.maxVolume = maxv;
    }

    public int getMaxVolume(){
        return this.maxVolume;
    }

    public int getVolume(){
        return this.volume;
    }

    public void setVolume(int v){
        this.volume = v;
    }

    public int empty(){
        int toMove = this.volume;
        this.volume = 0;
        return toMove;
    }

    public int fill(){
        int toMove = this.maxVolume - this.volume;
        this.volume = maxVolume;
        return toMove;
    }

    public int pourInto(Barrel b){
        int toMove = b.maxVolume - b.volume;
        if(toMove > this.volume){
            toMove = this.volume;
        }
        b.volume = b.volume + toMove;
        this.volume -= toMove;
        return toMove;
    }
}
