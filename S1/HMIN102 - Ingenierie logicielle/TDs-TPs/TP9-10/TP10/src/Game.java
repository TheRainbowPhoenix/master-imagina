/**
 * Created by ecrvn on 12/21/2016.
 */
public abstract class Game {

    protected AbstractWordSequenceFactory wordSequenceFactory;

    protected WordSequence wordSequence;

    protected Assignment foundLetters;

    public String toString(){
        String str = new String();
        str += "Game difficulty: ##Word Sequence: " + this.wordSequence.toString() + "##Found letters: " + this.foundLetters.toString();
        return str;
    }
}
