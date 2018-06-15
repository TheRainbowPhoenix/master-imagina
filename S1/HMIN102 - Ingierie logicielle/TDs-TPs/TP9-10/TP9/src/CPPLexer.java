/**
 * Created by ecrvn on 12/16/2016.
 */
public class CPPLexer extends Lexer {

    @Override
    public ScannedText scan(ProgramText t) {
        System.out.println("I am scanning a C++ program text.");
        return new ScannedText();
    }
}
