import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Scanner;

/**
 * A simple class to test our knowledge base and run queries on it.
 */

public class KnowledgeBaseInterrogator {

    public static void main(String[] args) throws Exception {

        /*__________GENERATING KNOWLEDGE BASE__________*/
        System.out.println("GENERATING KNOWLEDGE BASE");
        String filename = "exo1.txt";
        BufferedReader fileReader = new BufferedReader(new FileReader(filename));
        KnowledgeBase knowledgeBase = new KnowledgeBase(fileReader);
        System.out.println(knowledgeBase.toString());

        /*__________RUNNING QUERIES______________*/
        Scanner inputScan = new Scanner(System.in);
        while (true) {
            System.out.println("\nType a query to run on the knowledge base, or \"print\" to show the knowledge base again, or \"quit\" to end the program");
            String in = inputScan.nextLine();
            switch (in) {
                case "print":
                    System.out.println(knowledgeBase.toString());
                    break;
                case "quit":
                    return;
                default:
                    System.out.println(new Homomorphisms(in, knowledgeBase.getSaturatedFactBase()).toString());
                    break;
            }
        }
    }
}
