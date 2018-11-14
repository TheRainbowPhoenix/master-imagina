package structure;

public class Application {
	
	public static void main(String[] args)
	{
		if(args.length < 1){
			System.out.println("Usage : Application <FILEPATH>");
			return;
		}

		try {

			KnowledgeBase kb = new KnowledgeBase(args[0]);

			System.out.println("Affichage base de connaissance");

			System.out.println(kb);

		} catch(Exception e) {
			e.printStackTrace();
		}

		/*
		// creation de la base de faits de 2 facons differentes
		FactBase bf = new FactBase("P");
		bf.addAtomWithoutCheck(new Atom("Q"));
		System.out.println("Base de faits initiale :\n"+bf);
	
		// creation de la base de regles
		RuleBase br = new RuleBase();
		br.addRule(new Rule("P;S"));
		br.addRule(new Rule("Q;S;R"));
		System.out.println("Base de regles :\n"+br);
		*/
	}
}
