/**
 * Created by ecrvn on 10/31/2016.
 */
public class Product {
    protected double price;
    protected String name;

    public Product(String name , double price){
        this.name = name;
        this.price = price;
    }

    public double rentPrice(){
        return this.price;
    }
}
