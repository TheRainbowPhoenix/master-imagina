package Decorateur;


public class App
{
    public static void main( String[] args )
    {
        System.out.println("Exercice 1 :\n");

            Produit lgv = new Produit("La grande vadrouill", 10.0);
            Client cl = new Client("Dupont");
            Compte cmt = new Compte(cl);

            System.out.println("CompteNormal : " + cmt.prixLocation(lgv));

            Compte cmt2 = new CompteAvecReduction(cl);
            System.out.println("CompteAvecReduction : " + cmt2.prixLocation(lgv));

            Compte cmt3 = new CompteAvecSeuil(cl);
            System.out.println("CompteAvecSeuil : " + cmt3.prixLocation(lgv));
            System.out.println("CompteAvecSeuil : " + cmt3.prixLocation(lgv));
            System.out.println("CompteAvecSeuil : " + cmt3.prixLocation(lgv));

            Produit r4 = new ProduitSolde("RockyIV", 10.0);
            System.out.println("CompteNormal+ProduitSolde : "  + cmt.prixLocation(r4));
            System.out.println("CompteAvecReduction+ProduitSolde : "  + cmt2.prixLocation(r4));

        System.out.println("\nExercice 2 :\n");

            System.out.println("Ajout DecorationReduction au compteNormal");
            cmt = new CompteReductionDecoration(cmt);
            System.out.println("CompteNormal : " + cmt.prixLocation(lgv));


            System.out.println("\nAjout DecorationSeuil au compteNormal");
            cmt = new CompteSeuilDecoration(cmt);
            System.out.println("CompteNormal : " + cmt.prixLocation(lgv));
            System.out.println("CompteNormal : " + cmt.prixLocation(lgv));
            System.out.println("CompteNormal : " + cmt.prixLocation(lgv));    }
}
