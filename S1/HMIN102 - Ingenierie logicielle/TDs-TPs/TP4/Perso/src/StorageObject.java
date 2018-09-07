/**
 * Created by ecrvn on 10/25/2016.
 */
public abstract class StorageObject{
    protected Directory parentDir;
    protected int basicSize;
    protected String name;

    public Directory getParentDir(){ return this.parentDir; }

    public StorageObject(String fileName , int size){
        this.parentDir = null;
        this.name = fileName;
        this.basicSize = size;
    }

    public StorageObject(String fileName , int size , Directory parent){
        this.parentDir = parent;
        this.name = fileName;
        this.basicSize = size;
        this.parentDir.add(this);
    }

    public abstract int size();

    public String absoluteAddress(){
        if(parentDir==null){
            return this.name;
        } else {
            return parentDir.absoluteAddress() + "/" + this.name;
        }
    }
}
