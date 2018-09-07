public class SortedDictionary extends AbstractDictionary implements IDictionary {
	public SortedDictionary() {}

	//builds a dictionary meant to hold n entries
	public SortedDictionary(int n) {
		this.keys = new Object [n];
		this.values = new Object [n];
	}

	//returns the index of the entry with the given key
	//all this method does is call the actual indexOf method
	//this method is only here to limit the number of parameters
	public int indexOf(Object key) {
		return indexOf(key, 0 , this.keys.length - 1);
	}

	//returns the index where a new entry should be inserted
	public int newIndexOf(Object key) {
		int index = 0;

		while(index < this.size() && this.keys[index].compareTo(key) < 0) {
			index++;
		}

		if(index >= this.keys.length) {
			Object [] newKeys = new Object [this.keys.length + 1];
			Object [] newValues = new Object [this.values.length + 1];

			for(int i = 0 ; i < index ; i++) {
				newKeys[i] = this.keys[i];
				newValues[i] = this.values[i];
			}

			for(int i = index + 1 ; i < newKeys.length; i++) {
				newKeys[i] = this.keys[i-1];
				newValues[i] = this.values[i-1];
			}
			this.keys = newKeys;
			this.values = newValues;

		} else {
			Object temp_key = this.keys[index];
			Object temp_value = this.values[index];
			for(int i = index + 1 ; i < this.keys.length ; i++) {
				this.keys[index] = this.keys[i];
				this.values[index] = this.values[i];
				this.keys[i] = temp_key;
				this.values[i] = temp_value;
				temp_key = this.keys[index];
				temp_value = this.values[index];
			}
		}
		return index;
	}

	//returns the index of the entry with the given key
	//searches the index via a split search
	public int indexOf(Object key, int begin , int end) {
		int middle = begin + end / 2;
		if(this.keys[middle] == key)
			return middle;
		else if (key.compareTo(this.keys[middle]) > 0)
			return indexOf(key , middle , end);
		else
			return indexOf(key , begin , middle);
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
}
