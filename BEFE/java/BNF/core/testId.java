package bferris.core;

import java.io.PrintStream;

public class testId {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test Id class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    String      ind;
    PrintStream out;
    Id          id;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing Id...");
    
    // Try base64 encoding
    id = new Id("{00010203-0405-0607-0809-0a0b0c0d0e0f}");
    //Id     id = new Id();
    
    out.println(ind+"id.toSring()  = "+id.toString());
    out.println(ind+"id.ToString() = "+id.ToString());
    out.println(ind+"id.ToBase64() = "+id.ToBase64());
    out.println();
    out.println(ind+"(new Id(\"AAECAwQFBgcICQoLDA0ODw\").toString() = "+(new Id("AAECAwQFBgcICQoLDA0ODw").toString()));
    out.println(ind+"(new Id()).ToBase64()                        = "+(new Id()).ToBase64());
    out.println(ind+"(new Id()).toString()                        = "+(new Id()).toString());
    out.println(ind+"                                                              ^");
    out.println(ind+"                                                              |");
    out.println(ind+"                                                              +-- Should have '4' here because");
    out.println(ind+"                                                                  it's a UUID version 4 (random)");
    
    for (int i=0;i<256;i++) {
      out.println(ind+"(new Id()).ToBase64()                        = "+(new Id()).ToBase64());
    }
  }

}
