import java.util.HashMap;

public class Archive extends Directory implements IArchive
//classe représentant une archive. (Elle hérite de Directory car elle possède toutes les caractéristiques d'un dossier : surtout la manipulation de collection qui permet la réutilisation de méthodes.
{
	public Archive(String nom)
	{
		super(nom) ;
	}
	
	public HashMap getContent()
	{
		HashMap<String,ElementStockage> map = new HashMap<String,ElementStockage>() ;
		
		for (ElementStockage s : listeDossier)
		{
			map.put(s.absoluteAdress(),s) ;
		}
		
		return map ;
	}
	
	public void extract()
	{
		for (ElementStockage s : listeDossier)
		{
			s.parent.add(s) ;
		}
	}
	
	
}
