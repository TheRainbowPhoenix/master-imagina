package compiler;

import java.io.File;

public class JavaFactory extends CompilerFactory {

	public JavaFactory() {
		supportedLanguage = "Java";
	}
	
	public Lexer createLexer(){return new JavaLexer();}
	public Parser createParser(){return new JavaParser();}
	public Generator createGenerator(){return new JVMGenerator();}
	public PrettyPrinter createPrettyPrinter(){return new JavaPrettyPrinter();}
	
	class JavaLexer extends Lexer{
		public ScanedText scan(ProgramText t){
			System.out.println("I am scanning a Java program text");
			return new ScanedText();
		}
	}

	class JavaParser extends Parser{
		public AST parse(ScanedText t){
			System.out.println("I am parsing a Java scaned text");
			return new AST();
		}
	}
		
	class JVMGenerator extends Generator{
		public File generate(AST a){
			System.out.println("I am generating a JVM program text from a Java AST");
			return new File("f.jvm");
		}
	}
	
	class JavaPrettyPrinter extends PrettyPrinter{
		public ProgramText prettyPrint(AST a){
			System.out.println("I am pretty printing a Java AST");
			return new ProgramText("blabla");
		}
	}
}
