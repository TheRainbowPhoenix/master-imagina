
public class Symlink extends ElementStockage
//classe reprsentant un lien symbolique
{
		private ElementStockage reference  ; //fait rfrence un autre lment de stockage
		
		public Symlink(String nom)
		{
			super(nom,0) ;
			reference=null ;
		}
		
		public Symlink(String nom,ElementStockage e)
		{
			super(nom,0) ;
			reference=e ;
		}
		
		public int size()
		//ici, il est choisi comme option que la taille d'un lien symbolique correspond  la taille de sa rfrence
		{
			if (reference==null)
			return 0 ;
			
			else return reference.size()  ;
		}
		
		public void cat()
		//affiche le symbole du lien puis l'adresse absoule de sa rfrence
		{
			System.out.println("ln -s "+reference.absoluteAdress()+" "+name) ;
		}
	
		
}
