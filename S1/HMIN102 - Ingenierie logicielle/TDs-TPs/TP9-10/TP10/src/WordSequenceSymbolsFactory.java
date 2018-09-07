/**
 * Created by ecrvn on 12/21/2016.
 */
public class WordSequenceSymbolsFactory extends AbstractWordSequenceFactory {
    public WordSequenceSymbols getWordSequence(String words){
        return new WordSequenceSymbols(words);
    }
}
