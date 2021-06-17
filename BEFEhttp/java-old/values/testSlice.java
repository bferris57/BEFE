package bferris.values;

import java.io.PrintStream;

public class testSlice {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test Slice class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}

	public static void testNew(String indent) {

		String      ind;
    PrintStream out;
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(indent+"New...");

    // Various forms of New()...
    out.println(ind+"Slice.New()            = "+Slice.New());
    out.println(ind+"Slice.New(null)        = "+Slice.New(null));
    out.println(ind+"Slice.New(1)           = "+Slice.New(1));
    out.println(ind+"Slice.New(null,2)      = "+Slice.New(null,2));
    out.println(ind+"Slice.New(1,2)         = "+Slice.New(1,2));
    out.println(ind+"Slice.New(2,1)         = "+Slice.New(2,1));
    out.println(ind+"Slice.New(null,null,1) = "+Slice.New(null,null,1));
    out.println(ind+"Slice.New(null,2,1)    = "+Slice.New(null,2,1));
    out.println(ind+"Slice.New(null,2,-1)   = "+Slice.New(null,2,-1));
    
	}

	public static void testNormalise(String indent) {

		String      ind;
    PrintStream out;

    Slice[] slices = {Slice.New()
    		             ,Slice.New(1)
    		             ,Slice.New(1,2)
    		             ,Slice.New(2,1)
    		             ,Slice.New(null,null,1)
    		             ,Slice.New(null,2,1)
    		             ,Slice.New(null,2,-1)
    		             ,Slice.New(-1,-5,-2)
    		             };
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(indent+"Normalise(7)...");

    // Various forms of New()...
    for (Slice slice : slices) {
    	String s;
    	s = slice.toString();
    	while (s.length() < 10) s = s + " ";
    	out.println(ind+s+"-> "+Slice.Copy(slice).Normalise(7).ToString());
    }
    
    out.println(indent+"Normalise(2)...");

    // Various forms of New()...
    for (Slice slice : slices) {
    	String s;
    	s = slice.toString();
    	while (s.length() < 10) s = s + " ";
    	out.println(ind+s+"-> "+Slice.Copy(slice).Normalise(2).ToString());
    }

	}

	public static void test(String indent) {

    PrintStream out;
    String      ind;
    
    out = System.out;
    ind = indent+"  ";
    
    out.println(indent+"***Testing Slice...");

    testNew(ind);
    testNormalise(ind);
    
  }


}
