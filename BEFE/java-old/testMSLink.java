package bferris;

import java.io.PrintStream;

public class testMSLink {

  //----------------------------------------------------------------------------
  //
  // Method: test_MSLink - Test Microsoft .lnk file class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    String       ind;
    PrintStream  out;
    MSLink       link;
    int          fname;
    String[]     fnames = {"c:\\users\\public\\desktop\\HxD.lnk"
    		                  ,"c:\\users\\bferris\\desktop\\Eclipse.lnk"
    		                  ,"c:\\users\\bferris\\desktop\\Command Prompt.lnk"
    		                  ,"c:\\users\\bferris\\desktop\\BeanShell.lnk"
    		                  ,"c:\\users\\bferris\\desktop\\Dropbox.lnk"
                          };
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing MSLink...");

    for (fname=0; fname < fnames.length; fname++) {
      link = new MSLink(fnames[fname]);
      out.println(ind+"Link File  : '"+fnames[fname]+"'");
      out.println(ind+"Resolves to: '"+link.GetFullPath()+"'");
    }
  }

}
