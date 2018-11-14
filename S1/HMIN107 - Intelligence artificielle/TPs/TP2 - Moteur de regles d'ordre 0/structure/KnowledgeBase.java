package structure;

import java.io.*;
import java.util.*;

public class KnowledgeBase {
	
	private FactBase initFactBase;
	private FactBase saturedFactBase;
	private RuleBase ruleBase;

	public KnowledgeBase() {
		initFactBase = new FactBase();
		saturedFactBase = new FactBase();
		ruleBase = new RuleBase();
	}


	public KnowledgeBase(String filepath) throws Exception {
		
		saturedFactBase = new FactBase();
		ruleBase = new RuleBase();
		
		BufferedReader readFile = new BufferedReader(new FileReader(filepath));
				String line = readFile.readLine();

		initFactBase = new FactBase(line);

		while((line = readFile.readLine()) != null){
			ruleBase.addRule(new Rule(line));
		}

		readFile.close();
	}

	public FactBase getInitFactBase(){
		return initFactBase;
	}

	public FactBase getSaturedFactBase(){
		return saturedFactBase;
	}

	public RuleBase getRuleBase(){
		return ruleBase;
	}

	public void forwardChaining(){
		saturedFactBase.clear();

		boolean fin = false;
		HashMap<Rule,Boolean> applied = new HashMap<Rule, Boolean>();

		for(Rule r : ruleBase){
			applied.put(r, false);
		}

		ArrayList<Atom> nouvFaits = new ArrayList<Atom>();

		while(!fin){

			for(Rule r : ruleBase){

				if(initFactBase.contains(r.getHypothesis())){
					// continuer ici
				}
			}
		}
	}

	public String toString(){
		return "Base de faits initiale :\n"+initFactBase+"\nBase de regles :\n"+ruleBase+"\n";
	}
}