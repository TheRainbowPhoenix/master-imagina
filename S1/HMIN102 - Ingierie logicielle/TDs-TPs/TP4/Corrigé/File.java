
public class File extends ElementStockage
//classe reprsentant un fichier
{
	private String contenu ; // possde un contenu
	
	public File(String nom)
	{
		super(nom,0) ;
		this.contenu="" ;
	}
	
	public File(String nom,String contenu)
	{
		super(nom,0) ;
		this.contenu=contenu ;
	}
	
	public int size()
	{
		return (contenu.length()) ; //la taille du fichier correspond  la taille de son contenu
	}
	
	public void cat()
	//affiche le contenu  l'cran du fichier
	{
		System.out.println(contenu) ;
	}
	
	public void setContents(String c)
	//change le contenu du fichier. Le contenu devient c
	{
		contenu = c ;
	}
	
	
}
