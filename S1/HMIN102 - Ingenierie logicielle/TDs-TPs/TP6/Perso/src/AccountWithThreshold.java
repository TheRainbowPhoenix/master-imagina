/**
 * Created by ecrvn on 10/31/2016.
 */
public class AccountWithThreshold extends AccountWrapper {
    private int threshold;
    private int nbRents;

    public AccountWithThreshold(Account account , int threshold){
        super(account);
        this.threshold = threshold;
        this.nbRents = 0;
    }

    @Override
    public double rentPrice(Product product){
        if(nbRents < threshold){
            nbRents++;
            return this.account.rentPrice(product);
        }else{
            nbRents = 0;
            return 0;
        }
    }

    @Override
    public String toString(){
        return this.account.toString() + "It needs " + (this.threshold - this.nbRents) + " more rents to get a free product. ";
    }
}
