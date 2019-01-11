package compiler;

import java.io.File;

public class CPPFactory extends CompilerFactory {

	public CPPFactory() {
		supportedLanguage = "C++";
	}
	
	public Lexer createLexer(){return new CPPLexer();}
	public Parser createParser(){return new CPPParser();}
	public Generator createGenerator(){return new JVMGenerator();}
	public PrettyPrinter createPrettyPrinter(){return new CPPPrettyPrinter();}
	
	class CPPLexer extends Lexer{
		public ScanedText scan(ProgramText t){
			System.out.println("I am scanning a C++ program text");
			return new ScanedText();
		}
	}

	class CPPParser extends Parser{
		public AST parse(ScanedText t){
			System.out.println("I am parsing a C++ scaned text");
			return new AST();
		}
	}
		
	class JVMGenerator extends Generator{
		public File generate(AST a){
			System.out.println("I am generating an assembler program text from a C++ AST");
			return new File("f.jvm");
		}
	}
	
	class CPPPrettyPrinter extends PrettyPrinter{
		public ProgramText prettyPrint(AST a){
			System.out.println("I am pretty printing a CPP AST");
			return new ProgramText("blabla");
		}
	}
}
