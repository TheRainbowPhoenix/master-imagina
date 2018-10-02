/**
 * Created by ecrvn on 10/31/2016.
 */
public class AccountWithDiscount extends AccountWrapper {
    private double discount;

    public AccountWithDiscount(Account account , int discount){
        super(account);
        this.discount = discount;
    }

    @Override
    public double rentPrice(Product product){
        return this.account.rentPrice(product) * ((100 - discount) / 100);
    }

    @Override
    public String toString(){
        return this.account.toString() + "It has a discount of " + this.discount + "%. ";
    }
}
