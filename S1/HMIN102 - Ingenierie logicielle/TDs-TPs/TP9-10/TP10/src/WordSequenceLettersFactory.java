/**
 * Created by ecrvn on 12/21/2016.
 */
public class WordSequenceLettersFactory extends AbstractWordSequenceFactory{
    public WordSequenceLetters getWordSequence(String words){
        return new WordSequenceLetters(words);
    }
}
