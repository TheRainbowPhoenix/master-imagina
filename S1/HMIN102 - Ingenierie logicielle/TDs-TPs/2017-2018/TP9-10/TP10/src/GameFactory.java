/**
 * Created by ecrvn on 12/21/2016.
 */
public abstract class GameFactory {

    public static Game startGame(String difficulty) {
        if (difficulty.equalsIgnoreCase("EASY")) {
            System.out.println("Starting easy game!");
            return new EasyGame();
        } else if (difficulty.equalsIgnoreCase("MEDIUM")) {
            System.out.println("Starting medium game!");
            return new MediumGame();
        } else if (difficulty.equalsIgnoreCase("HARD")) {
            System.out.println("Starting hard game!");
            return new HardGame();
        }
        System.out.println("Difficulty not recognized.");
        return null;
    }
}
