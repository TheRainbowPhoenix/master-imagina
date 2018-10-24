package csp;

import java.io.*;
import java.io.BufferedReader;
import java.util.ArrayList;

public class Application {

	public static void main(String[] args){

		try {
			if(args.length > 0) {
				
				String filename = args[0];

				Network network;
				CSP csp;

				Assignment firstSolution;
				ArrayList<Assignment> allSolutions;

				System.out.println("Chargement du fichier :" + new java.io.File(".").getCanonicalPath()+ "/" + filename);
				System.out.println("");
					
				BufferedReader fileReader = new BufferedReader(new FileReader(filename));
				
				network = new Network(fileReader);
				csp = new CSP(network);
				

				firstSolution = csp.searchSolution();
				
				System.out.println("Bactrack solution trouvée : " + firstSolution);
				System.out.println("Nombre de noeuds explorés : " + csp.cptr);
				System.out.println("");
				
				allSolutions = csp.searchAllSolutions();
				
				System.out.println("BacktrackAll solutions trouvées : " + allSolutions);
				System.out.println("Nombre de noeuds explorés : " + csp.cptr);
				System.out.println("");

				fileReader.close();
			}
			else{
				System.out.println("Usage : " + System.getProperty("sun.java.command") + " <File>");
			}


		} catch(Exception e) {
			System.out.println("Erreur lors du chargement: " + e.getMessage());
		}
	}
}