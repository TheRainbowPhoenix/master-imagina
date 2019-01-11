
public class Test {

	public static void main(String[] args){
		ComposantOrdi m1 = new RAM("Corsair");
		Montage m2 = new Montage();
		ComposantOrdi m3 = new Montage();

		m2.equiv(m1,"x");
		m2.equiv(m2,"x");
		m2.equiv(m3,"x");
		m2.equiv((Montage) m3,"x");
		m3.equiv(m1,"x");
		m3.equiv(m2,"x");
		m3.equiv(m3,"x");

		m3.equiv((Montage)m3,"x");

	}
}
