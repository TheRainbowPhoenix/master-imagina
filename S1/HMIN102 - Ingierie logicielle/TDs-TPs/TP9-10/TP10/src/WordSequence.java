/**
 * Created by ecrvn on 12/21/2016.
 */
public class WordSequence {

    String words;

    Assignment textTransform;

    public WordSequence(String words){
        this.words = words;
        this.textTransform = new Assignment();
        this.textTransform.put('a' , 'a');
    }

    public String toString(){
        String str = new String();
        for(int i = 0 ; i < this.words.length() ; i++){
            Character character = this.words.charAt(i);
            if(this.textTransform.keySet().contains(character))
                str += this.textTransform.get(character);
            else
                str += character;
        }
        return str;
    }
}
