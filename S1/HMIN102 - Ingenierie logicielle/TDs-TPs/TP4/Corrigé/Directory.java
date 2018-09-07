import java.util.ArrayList;
import java.util.Collection;

public class Directory extends ElementStockage
//classe reprsentant un dossier
{
	Collection<ElementStockage> listeDossier ; // collection des lments de stockage que le dossier contient
		
	public Directory(String nom)
	{
		super(nom,4) ;
			
		listeDossier = new ArrayList<ElementStockage>() ;
	}
		
	public int size()
	//ici, la taille d'un dossier est gale  la somme des tailles des diffrents lments prsents dans ce dossier.
	{
		int somme=basicSize ;
			
		for (ElementStockage s : listeDossier) // parcours de la collection (parcours de tout le dossier)
		{
			somme=somme+s.size() ;
		}
			
		return somme ;
	}
		
	public void ls()
	//affiche  l'cran la liste des lments qu'il contient
	{
		for (ElementStockage s : listeDossier)
		{
			System.out.println(s.name) ;
		}
	}
		
	public int nbElm()
	//retourne le nombre d'lments du dossier
	{
		return listeDossier.size() ;
	}
		
	public boolean add(ElementStockage e)
	//ajoute l'lment e dans le dossier
	{
		e.nouveauParent(this) ; // le rpertoire courant devient le pre de l'lment e
		return listeDossier.add(e) ; //ajout de l'lment e  la collection
	}
		
	public boolean remove(ElementStockage e)
	//supprime et retourne vrai si la suppression de l'lment e s'est bien passe, faux sinon (e non prsent ou erreur)
	{
		for (ElementStockage s : listeDossier)
		{
			if (s.name==e.name)
			return listeDossier.remove(s) ; // suppression de l'lment e dans la collection
		}
			
		return false ;
	}
	
	public boolean include(ElementStockage e)
	{
		return listeDossier.contains(e) ;
	}
	
	public ElementStockage findElement(String nom)
	//rend l'element de nom "nom" si il existe (sinon retourne null)
	{
		for (ElementStockage s : listeDossier)
		{
			if (s.name.equals(nom))
			return s ;
		}
		
		return null ;
	}
		
	public ArrayList<String> find(String nom)
	//rend la collection des adresses absolues de nom "nom" que le repertoire contient
	{
		ArrayList<String> collection = new ArrayList<String>() ;
			
		for (ElementStockage s : listeDossier)
		{
			if (s.name.equals(nom))
			collection.add(s.absoluteAdress()) ;
		}
			
		return collection ;
	}
		
	public ArrayList<String> findR(String nom)
	//rend la collection des adresses absolues de nom "nom" que le rpertoire contient directement ou par transitivit
	{
		ArrayList<String> collection = new ArrayList<String>() ;
		ArrayList<String> temporaire ; // collection temporaire servant  stocker les adresses absolues obtenues par transitivit
			
		for (ElementStockage s : listeDossier)
		{
			if (s.name==nom)
			collection.add(s.absoluteAdress()) ;
			
			if (s instanceof Directory) // si l'lment de stockag est un dossier, lance la recherche sur s (appel rcursif)
			{
				temporaire = ((Directory) s).findR(nom) ;
			
				for (String es : temporaire)
				collection.add(es) ;
			}
		}
		
		return collection ;
	}
	
	public void afficheCollection(ArrayList<String> liste)
	//affiche le contenu de la collection liste
	{
		for (String s : liste)
		System.out.println(s) ;
	}
	
	
}
	
	
	
	
	
	

