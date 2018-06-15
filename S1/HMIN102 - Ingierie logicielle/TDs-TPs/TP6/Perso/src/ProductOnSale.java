/**
 * Created by ecrvn on 11/2/2016.
 */
public class ProductOnSale extends ProductWrapper {
    protected double discount;

    public ProductOnSale(Product product , int discount){
        super(product);
        this.discount = discount;
    }

    @Override
    public double rentPrice() {
        return super.rentPrice() * ((100 - this.discount) / 100);
    }
}
