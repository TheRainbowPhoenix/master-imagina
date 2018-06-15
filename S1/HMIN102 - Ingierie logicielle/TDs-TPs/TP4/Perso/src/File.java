import java.util.ArrayList;

/**
 * Created by ecrvn on 10/25/2016.
 */
public class File extends StorageObject{
    protected String contents;

    public File(String fileName, String text , Directory parent) {
        super(fileName , 0 , parent);
        this.contents = text;
    }

    @Override
    public int size() {
        return this.contents.length();
    }

    public void cat(){
        System.out.println(this.contents);
    }

    public int nbElem(){
        String [] words = this.contents.split(" ");
        return words.length;
    }
}
