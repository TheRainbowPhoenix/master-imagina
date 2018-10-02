/**
 * Created by ecrvn on 12/16/2016.
 */
public class Compiler {

    protected Lexer lexer;

    protected Parser parser;

    protected Generator generator;

    public Compiler(String language) {
        if (language.equals("Java")) {
            this.lexer = new JavaLexer();
            this.parser = new JavaParser();
            this.generator = new JavaGenerator();
        } else if (language.equals("C++")) {
            this.lexer = new CPPLexer();
            this.parser = new CPPParser();
            this.generator = new CPPGenerator();
        } else {
            System.out.println("Non supported language: " + language + ", extend the framework to support it.");
            return;
        }
    }

    public File compile(ProgramText t) {
        File file = this.generator.generate(this.parser.parse(this.lexer.scan(t)));
        System.out.println("Compilation finished.");
        return file;
    }
}
