import java.util.ArrayList;

public abstract class AbstractDictionary implements IDictionary {
	protected Object [] keys,
						values;
	
	//returns a boolean telling us whether the dictionary is empty or not
	public boolean isEmpty() {
		return keys.length <= 0;
	}
	
	//returns a boolean telling us whether the dictionary contains an item with the given key
	public boolean containsKey(Object key) {
		
		for(int i = 0 ; i < keys.length ; i++) {
			if(keys[i] == key) return true;
		}
		return false;
	}
	
	//returns the index where the item with given key is placed
	public abstract int indexOf(Object key);
	
	//returns the index where the new item with given key will be placed
	public abstract int newIndexOf(Object key);
	
	//returns the length of the dictionary
	public int size() {
		return keys.length;
	}
	
	//returns the object corresponding to the given key
	public Object get(Object key) {
		if(this.containsKey(key)) return this.values[this.indexOf(key)];
		else return null;
	}
	
	//inserts a new item in the dictionary with the given key and value
	public Object put(Object key , Object value) {
		int i = this.newIndexOf(key);
		this.keys[i] = key;
		this.values[i] = value;
		return this;
	}
}
