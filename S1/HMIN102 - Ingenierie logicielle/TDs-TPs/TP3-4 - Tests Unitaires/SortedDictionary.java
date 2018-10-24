public class SortedDictionary extends OrderedDictionary {
	// protected Comparable[] keys; En Java, on ne peut pas redefinir un
	// attribut. cf commentaire ci-dessous.

	/**
	 * Pour le SortedDictionary il faut que les elts soient comparables afin
	 * d'etablir un ordre. - Soit on redefinit l'attribut keys
	 * "protected Comparable [] keys" et le parametre key de la methode
	 * "int newIndexOf(Obkect key)" peut rester un Object en faisant les
	 * comparaisons entre cles comme suit : keys[i].compareTo(key) (cf. #).
	 * 
	 * --> Attention : Apres test, il apparait que les attributs ne sont
	 * pas redefinissable en JAVA mais seulement surchargeable. Avec ce qui
	 * est enonce ci-dessus, on se retrouve avec deux tableaux "keys" : un
	 * tableau d'objets et un tableau de comparable. A priori, la solution
	 * ci-dessus aurait par contre ete faisable en C++.
	 * 
	 * - Soit on ne redefinit pas l'attribut "keys[]" et il faut ecrire la
	 * methode "newIndexOf" ainsi : int newIndexOf(Object key) { Comparable
	 * comp = (Comparable) key; ... comp.compareTo(keys[i]); ... }
	 * 
	 * # int compareTo(Object); --> Interface Comparable
	 * 
	 */

	int newIndexOf(Object key) {
		//la cle doit etre un element de type comparable mais impossible de changer la signature de la methode
		Comparable cKey = (Comparable) key;
		//creation de nouveaux tableaux plus grands de 1
		Object[] newKeys = new Object[keys.length + 1];
		Object[] newValues = new Object[keys.length + 1];
		int i = 0, ret = 0;
		//recherche de la position de la derniere cle plus petite que ckey
		while (i < keys.length && cKey.compareTo(keys[i]) > 0) { // cKey > keys[i]
			newKeys[i] = keys[i];
			newValues[i] = values[i];
			i++;
		}
		//ret est l'indice auquel se fera l'insertion
		ret = i;
		newKeys[i] = null;
		newValues[i] = null;
		//recopie des autres elements
		if (i != keys.length) {
			for (int j = i + 1; j < newKeys.length; j++) {
				newKeys[j] = keys[j - 1];
				newValues[j] = values[j - 1];
			}
		}
		keys = newKeys;
		values = newValues;
		return ret;
	}
}
