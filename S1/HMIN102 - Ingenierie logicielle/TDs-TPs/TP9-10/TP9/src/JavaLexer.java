/**
 * Created by ecrvn on 12/16/2016.
 */
public class JavaLexer extends Lexer {

    @Override
    public ScannedText scan(ProgramText t) {
        System.out.println("I am scanning a Java program text.");
        return new ScannedText();
    }
}
