/**
 * Created by ecrvn on 11/2/2016.
 */
public abstract class ProductWrapper extends Product{
    protected Product product;

    public ProductWrapper(Product product){
        super(product.name , product.price);
        this.product = product;
    }
}
