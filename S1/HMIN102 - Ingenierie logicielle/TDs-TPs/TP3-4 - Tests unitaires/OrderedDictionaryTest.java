import org.junit.*;
import static org.junit.Assert.*;
import org.mockito.*;

public class OrderedDictionaryTest {

	private OrderedDictionary OD;
	@Mock I mock;

	@Before
	public void setUp() throws Exception {
		OD = new OrderedDictionary();
	}

	@Test
	public void testAddOneElementToEmptyDico(){
		try {

			OD.put("key", "value");
			System.out.println("\nkey value : " + OD.get("key"));

			assertEquals(1, OD.size());
			assertTrue(OD.containsKey("key"));
			assertEquals(OD.get("key"), "value");

		} catch(Exception e) {
			System.out.println(e.getMessage());
		}
	}


	@Test
	public void testAddManyElementsToEmptyDico(){
		try {

			int size = 1000;

			for(int i =  0 ; i < size ; i++) {
				OD.put(i, "value " + String.valueOf(i));
			}

			assertEquals(size, OD.size());

			for(int i =  0 ; i < size ; i++) {
				assertEquals("value " + i, OD.get(i));
			}

		} catch(Exception e) {
			System.out.println(e.getMessage());
		}
	}

	@Test
	public void mockTest() {

		try {

			assertEquals(0, mock.methodInt());
			assertEquals(0, mock.methodInt());
			assertEquals(0, mock.methodInt());
			assertEquals(1, mock.methodInt());

		} catch(Exception e) {
			System.out.println(e.getMessage());
		}
	}
}