package csp_etud;

import java.io.*;
import java.io.BufferedReader;

public class Application {

	public static void main(String[] args){

		try {

			String filename = "color_problem.csp";
			Network network;

			System.out.println("Chargement du fichier :" + new java.io.File(".").getCanonicalPath()+ "/" + filename);
			
			
			BufferedReader readFile = new BufferedReader(new FileReader(filename));
			network = new Network(readFile);
			
			readFile.close();

		} catch(Exception e) {
			System.out.println("Chargement " + e.getMessage());
		}
	}
}