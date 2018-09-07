import java.util.Scanner;

/**
 * Created by ecrvn on 12/24/2016.
 */
public class Test {

    public static void main(String[] args){
        Scanner userInput = new Scanner(System.in);
        String input = new String();
        while(true){
            System.out.println("Which difficulty level do you want to play? EASY/MEDIUM/HARD // quit");
            input = userInput.nextLine();
            if(input.equalsIgnoreCase("QUIT"))
                return;
            Game game = GameFactory.startGame(input);

        }
    }
}
