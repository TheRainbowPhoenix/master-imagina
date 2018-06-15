/**
 * Created by ecrvn on 12/21/2016.
 */
public class EasyGame extends Game {

    public EasyGame(){
        this.foundLetters = new Assignment();
        this.wordSequenceFactory = new WordSequenceSymbolsFactory();
        this.wordSequence = this.wordSequenceFactory.getWordSequence("The quick brown fox jumps over the lazy dog");
        this.toString();
        System.out.println(this.toString());
    }

    public void playTurn(){

    }

    public String toString(){
        String str = super.toString();
        String[] tokens = str.split("##");
        str = tokens[0] + "Easy\n" + tokens[1] + "\n" + tokens[2];
        ///System.out.print(tokens[0] + "|||" + tokens[1]);
        return str;
    }
}
