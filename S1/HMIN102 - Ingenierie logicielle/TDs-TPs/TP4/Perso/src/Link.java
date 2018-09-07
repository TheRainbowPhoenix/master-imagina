/**
 * Created by ecrvn on 10/25/2016.
 */
public class Link extends StorageObject{
    private StorageObject reference;

    public Link(String fileName, StorageObject linksTo , Directory parent) {
        super(fileName , 0 , parent);
        this.reference = linksTo;
    }

    @Override
    public int size() {
        return reference.size();
    }

    public void cat(){
        System.out.println("ln -s " + this.reference.absoluteAddress() + " " + this.name);
    }
}
