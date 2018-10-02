
public class Link extends ElementStockage 
{
	private String contenu ;
	
	public Link(String nom)
	{
		super(nom,0) ;
		this.contenu="" ;
	}
	
	public Link(String nom,String contenu)
	{
		super(nom,0) ;
		this.contenu=contenu ;
	}
	
	public int size()
	{
		return contenu.length()  ;
	}
}
