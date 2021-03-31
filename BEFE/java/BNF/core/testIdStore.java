package bferris.core;

import java.io.PrintStream;

public class testIdStore {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test Id class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    //String      ind;
    PrintStream out;
    
    //ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing IdStore...");
    
    IdStore.New();
  }


}
