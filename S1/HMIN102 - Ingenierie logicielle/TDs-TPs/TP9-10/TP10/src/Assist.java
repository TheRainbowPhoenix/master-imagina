/**
 * Created by ecrvn on 12/21/2016.
 */
public abstract class Assist {

    public static Assignment assist(Assignment assignment){
        Character key = (Character) assignment.keySet().toArray()[0];
        Character value = (Character) assignment.get(key);
        assignment.remove(key);
        Assignment hint = new Assignment();
        hint.put(key , value);
        return hint;
    }
}
