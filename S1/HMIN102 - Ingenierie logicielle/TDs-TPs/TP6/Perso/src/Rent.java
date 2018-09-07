/**
 * Created by ecrvn on 10/31/2016.
 */
public class Rent {
    public static void main(String[] args){
        Product pr1 = new Product("La grande vadrouille" , 10.0);
        Product pr2 = new Product("Les gendarmes à St-Tropez" , 13.0);
        Product pr3 = new ProductOnSale(new Product("Les gendarmes à St-Tropez" , 15.0) , 23);
        Customer cu = new Customer("Dupont");
        Account acct = new AccountWithDiscount(new AccountWithThreshold(new Account(cu) , 2) , 10);
        System.out.println(acct.toString());
        System.out.println(acct.rentPrice(pr1));
        System.out.println(acct.toString());
        System.out.println(acct.rentPrice(pr2));
        System.out.println(acct.toString());
        System.out.println(acct.rentPrice(pr3));
    }
}
