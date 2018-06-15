/**
 * Created by ecrvn on 12/16/2016.
 */
public class JavaParser extends Parser {

    @Override
    public AST parse(ScannedText t) {
        System.out.println("I am parsing a Java scanned text and I generate a Java AbstractSyntaxTree.");
        return new AST();
    }
}
