import java.util.Stack;

/**
 * Created by ecrvn on 10/27/2016.
 */
public class Game {
    private Stack<Command> history;

    public Game(){
        history = new Stack<Command>();
    }

    public void runCommand(Command c){
        c.execute();
        this.history.push(c);
    }

    public void cancelCommand(){
        (this.history.pop()).cancel();
    }

    /**
     *
     * fction de debug pour verifier l'etat de bidons
     *
     * @param b1
     * @param b2
     * @param b1
     * @return
     */
    public static String etatBarrel(Barrel b1,Barrel b2,Barrel b3){
        return b1.getVolume() + " "+b2.getVolume()+" "+b3.getVolume();
    }
    public static void main(String[] args) {

        Game game = new Game();

        Barrel b1 = new Barrel(3);
        Barrel b2 = new Barrel(4);
        Barrel b3 = new Barrel(6);

        game.runCommand(new FillBarrel(b1));
        //game.runCommand(new FillBarrel(b2));
        game.runCommand(new FillBarrel(b3));
        System.out.println(etatBarrel(b1, b2, b3));

        game.runCommand(new PourBarrel(b1,b2));
        System.out.println(etatBarrel(b1, b2, b3));

        game.runCommand(new PourBarrel(b3,b1));
        System.out.println(etatBarrel(b1, b2, b3));

        game.runCommand(new PourBarrel(b2,b3));
        System.out.println(etatBarrel(b1, b2, b3));

        game.runCommand(new EmptyBarrel(b3));
        System.out.println(etatBarrel(b1, b2, b3));

        System.out.println("maintenant on annule");
        while(!game.history.empty()){
            game.cancelCommand();
            System.out.println(etatBarrel(b1, b2, b3));
        }
    }
}
