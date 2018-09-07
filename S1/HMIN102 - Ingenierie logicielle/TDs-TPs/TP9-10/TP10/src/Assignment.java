import java.util.HashMap;

/**
 * Created by ecrvn on 12/21/2016.
 */
public class Assignment extends HashMap<Character,Character> {

    public String toString(){
        String str = new String();
        for(Character key : this.keySet()){
            str += key + " -> " + this.get(key) + "\n";
        }
        return str.substring(0 , str.length());
    }
}
