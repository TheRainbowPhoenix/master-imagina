import static org.mockito.Mockito.*;
import static org.junit.Assert.*;


public class Test{

	public static void main(String[] args) throws Exception{
		I mockI = mock(I.class);	

		/**
		 * QUESTION 1
		 **/
		
		//assert that methodInt returns 0
		try {
			assert(mockI.methodInt() == 0);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		//make mockI return 1,2,3 and then 4 on successive methodInt() calls
		when(mockI.methodInt()).thenReturn(1)
		.thenReturn(2)
		.thenReturn(3)
		.thenReturn(4);
		System.out.println(mockI.methodInt());
		System.out.println(mockI.methodInt());
		System.out.println(mockI.methodInt());
		System.out.println(mockI.methodInt());
		verify(mockI , times(4)).methodInt();

		//make mockI throw Exception on calling methodVoid()
		doThrow(new Exception()).when(mockI).methodVoid();
		//mockI.methodVoid();
		
		//mockI methodParam()
		when(mockI.methodParam(3)).thenReturn(3);
		when(mockI.methodParam(7)).thenReturn(10);
		try{
			assertEquals(mockI.methodParam(0) , 0);
			assertEquals(mockI.methodParam(3) , 3);
			assertEquals(mockI.methodParam(7) , 10);
		}catch(Exception e){
			e.printStackTrace();
		}
		
		/**
		 * QUESTION 2
		 **/
		//playing around with spies
		A a = new A();
		A spyA = spy(A.class);
		a.m1();
		spyA.m1();
		a.m2(2);
		spyA.m2(2);
		try{
			assertEquals(spyA.m1() , a.m1());
			verify(spyA , atLeast(1)).m1();
			verify(spyA , atLeast(1)).m2(anyInt());
		}catch(Exception e){
			e.printStackTrace();
		}
		
		//mock method m2()
		when(spyA.m2(42)).thenReturn(0);
		try{
			assertEquals(spyA.m2(42) , 0);
		}catch(Exception e){
			e.printStackTrace();
		}
	}	
}