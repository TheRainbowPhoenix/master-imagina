import java.util.ArrayList;
import java.util.Collection;

/**
 * Created by ecrvn on 10/25/2016.
 */
public class Directory extends StorageObject{
    private Collection<StorageObject> contents;

    public Directory(String fileName){
        super(fileName,4);
        contents = new ArrayList<>();
    }

    public Directory(String fileName , Directory parent){
        super(fileName,4,parent);
        contents = new ArrayList<>();
    }

    @Override
    public int size(){
        int totalSize = basicSize;
        for(StorageObject so : contents)
            totalSize += so.size();
        return totalSize;
    }

    public void ls(){
        for(StorageObject so : contents)
            System.out.println(so.name);
    }

    public int nbElem(){
        return contents.size();
    }

    public void add(StorageObject so){
        this.contents.add(so);
    }

    public ArrayList<String> find(String name){
        ArrayList<String> addresses = new ArrayList<String>();
        for(StorageObject so : contents)
            if((so.name).equals(name))
                addresses.add(so.absoluteAddress());
        return addresses;
    }

    public ArrayList<String> findR(String name){
        ArrayList<String> addresses = new ArrayList<String>();
        for(StorageObject so : contents){
            if((so.name).equals(name)){
                //System.out.println(so.absoluteAddress());
                addresses.add(so.absoluteAddress());
            }
            if(so instanceof Directory){
                addresses.addAll(((Directory)so).findR(name));
            }
        }
        return addresses;
    }
}
