/**
 * Created by ecrvn on 10/31/2016.
 */
public abstract class AccountWrapper extends Account {
    protected Account account;

    public AccountWrapper(Account account){
        super(account.customer);
        this.account = account;
    }


}
