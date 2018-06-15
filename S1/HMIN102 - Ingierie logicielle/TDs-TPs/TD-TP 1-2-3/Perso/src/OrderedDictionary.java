import java.util.ArrayList;

public class OrderedDictionary extends AbstractDictionary implements IDictionary {

	public OrderedDictionary(){
		this.keys = new Object [0];
		this.values = new Object [0];
	}
	
	public OrderedDictionary(int n) {
		this.keys = new Object [n];
		this.values = new Object [n];
	}

	//returns the index of the entry with the given key
	public int indexOf(Object key) {
		
		for(int i = 0 ; i < this.keys.length ; i++) {
			if(key == keys[i]) return i;
		}
		return -1;
	}

	//returns the index where a new entry should be inserted
	public int newIndexOf(Object key) {
		Object [] newKeys = new Object [this.keys.length + 1];
		Object [] newValues = new Object [this.values.length + 1];
		
		for(int i = 0 ; i < this.values.length ; i++) {
			newKeys[i] = this.keys[i];
			newValues[i] = this.values[i];
		}
		this.keys = newKeys;
		this.values = newValues;
		return this.keys.length - 1;
	}
}