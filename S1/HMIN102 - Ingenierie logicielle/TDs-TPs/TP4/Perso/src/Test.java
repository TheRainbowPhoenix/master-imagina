import java.util.ArrayList;

/**
 * Created by ecrvn on 10/25/2016.
 */
public class Test {
    public static void main(String [] args) {
        Directory root = new Directory("root");
        Directory dir1 = new Directory("file" , root);
        File f1 = new File("file" , "Test how many words are in this file" , root);
        File f2 = new File("file" , "Empty file" , dir1);
        Link l1 = new Link("file" , f1 , dir1);
        ArrayList<String> list = root.findR("file");

        l1.cat();
        f1.cat();
        System.out.println(f1.absoluteAddress());
        System.out.println(l1.absoluteAddress());
        System.out.println(dir1.absoluteAddress());
        System.out.println(f1.nbElem());
        System.out.println(dir1.nbElem());
        root.ls();
    }
}
