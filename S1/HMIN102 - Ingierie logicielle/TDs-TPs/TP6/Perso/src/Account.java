/**
 * Created by ecrvn on 10/31/2016.
 */
public class Account{
    protected Customer customer;

    public Account(Customer customer){
        this.customer = customer;
    }

    public double rentPrice(Product product){
        return product.rentPrice();
    }

    public String toString(){
        return "This is " + this.customer.toString() + "'s account. ";
    }
}
