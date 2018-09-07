import java.util.HashMap;

public class FastDictionary extends AbstractDictionary implements IDictionary {

	public FastDictionary() {
		this.keys = new Object [0];
		this.values = new Object [0];
	}

	//builds a new dictionary meant to hold n entries
	//that is not equivalent to a dictionary of size n
	//but it means it will only need to grow after inserting n entries
	public FastDictionary(int n) {
		int length = n * 3 / 4;
		this.keys = new Object [length];
		this.values = new Object [length];
	}

	//returns the index of the entry with the given key
	public int indexOf(Object key){
		int index = key.hashCode() % this.size();
		while(this.keys[index] != key){
			index++;
		}
		return index;
	}

	//returns the index where a new entry should be inserted
	public int newIndexOf(Object key){
		int index = key.hashCode() % this.size();
		if(this.mustGrow())
			this.grow();
		while(this.keys[index] != null){
			index++;
		}
	return index;
	}

	//returns the number of entries
	public int size(){
		int size = 0;
		for(int i = 0 ; i < this.keys.length ; i++){
			if(this.keys[i] != null)
				size++;
		}
		return size;
	}

	//determines whether the array size should be increased
	//(if the dictionary is more than 3/4 full)
	public boolean mustGrow(){
		return this.size() >= (this.keys.length * 3 / 4);
	}

	//increases the array size so that the dictionary is 3/4 full
	public void grow(){

	}
}
