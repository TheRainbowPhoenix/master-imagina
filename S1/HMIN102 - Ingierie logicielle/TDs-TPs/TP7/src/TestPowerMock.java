import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;
import static org.mockito.Mockito.*;
import static org.junit.Assert.*;

@RunWith(PowerMockRunner.class)
@PrepareForTest({A.class})
public class TestPowermock{
	void test(){
		@Test
		public void mockFinalTest() {
			A mockA = PowerMockito.mock(A.class);
			final String resultMock = "mocked value";
			when(mockA.m1())).thenReturn(resultMock);
			assertEquals(mockA.m1() , resultMock);
		}
	}
}
