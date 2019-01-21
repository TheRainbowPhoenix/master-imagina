# 1. Framework 

```java
public interface Produit {

	Produit(double prix);
	double prixHT();
	double prixTTC();
}

public class Livre extends Produit {

}

public class DVD extends Produit {

}

public class K7 extends Produit {

}

