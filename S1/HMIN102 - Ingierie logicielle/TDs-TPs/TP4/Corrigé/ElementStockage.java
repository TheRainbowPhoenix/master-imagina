
public abstract class ElementStockage 
//classe abstraite correspondant  tous lment de stockage mmoire.
{
	protected String name ; // un nom
	protected int basicSize ; //espace de base qu'il occupe en memoire
	protected Directory parent ; // dossier parent 
	
	public ElementStockage(String nom,int t)
	{
		name=nom ;
		basicSize=t ;
		parent=null ;
	}
	
	public ElementStockage(String nom,int t,Directory d)
	{
		name=nom ;
		basicSize=t ;
		parent=d ;
	}
	
	public abstract int size() ; // classe abstraite car spcifique diffrentes paramtrisations (retourne la taille de l'lment de stockage)
	
	
	public String absoluteAdress()
	//retourne l'adresse absolue de l'lment.
	{
		if (parent != null) // s'il a un parent
		return (parent.absoluteAdress()+"/"+name) ;
		
		else return (name) ; // ici on est  la racine, on suppose que son nom est racine 
	}
	
	public void nouveauParent(Directory d)
	//d devient le nouveau parent de l'lmnet de stockage.
	{
		parent=d ;
	}
	
	public int bitSize()
	//retourne la taille en bits de l'lment.
	{
		return this.size()*8 ;
	}
	
	public abstract void accept(Visitor v) ;
}
