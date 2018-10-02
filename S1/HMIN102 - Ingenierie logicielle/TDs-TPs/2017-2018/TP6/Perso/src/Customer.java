/**
 * Created by ecrvn on 10/31/2016.
 */
public class Customer {
    private Account account;
    private String name;

    public Customer(String name){
        this.name = name;
    }

    public Customer(String name, Account account){
        this.name = name;
        this.account = account;
    }

    public void setAccount(Account account){
        this.account = account;
    }

    public String toString(){
        return this.name;
    }
}
