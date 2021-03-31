package bferris.abnf;

import java.io.File;
import java.io.PrintStream;
import bferris.util.ReadFile;

import bferris.json.JSON;
import bferris.json.JSONObject;

public class testABNF {

	//----------------------------------------------------------------------------
  //
  // Method: test - Test ABNF... classes
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

  	String      ind;
    PrintStream out;
    boolean     passed = false;
    int         i;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing ABNF...");
    
    while (true) {
    	
		//
		// Do all the tests
		//
		
 		  // Test an entire file
    	String     root = "d:\\cloud\\dropbox\\SVN\\bferris\\bferris\\testfiles\\";
  		String     fname;
  		File       afile;
  		String     astring;
  		ABNF       abnf;
  		String     jsonstring;
  		JSON       json;
  		JSONObject tokens;
  		String[]   tokennames;
  		
      // TEMP: Test ABNFTerminal to string
      //out.println(ind+"Testing ABNFTerminal...");
      //out.println(ind+ind+"Terminal \"abc\" = "+(new ABNFTerminal("abc")));
      //out.println(ind+ind+"Terminal CRLF  = "+(new ABNFTerminal("\r\n")));
      //out.println(ind+ind+"Terminal 0x0d  = "+(new ABNFTerminal(0x0d)));
      //out.println(ind+ind+"Terminal '\\r'  = "+(new ABNFTerminal('\r')));
      
      // Test file
      out.println(ind+"Testing .abnf file...");
      //fname = root+"test.abnf";
      //fname = root+"abnf.abnf";
      fname = root+"json.abnf";
      afile = new File(fname);
      if (!afile.exists()) {
      	out.println(ind+ind+"File '"+fname+"' does not exist");
      	passed = false;
      	break;
      }
      astring = ReadFile.ReadString(afile);
      abnf = ABNF.Parse(astring);
      if (abnf.IsError()) {
      	out.println(ind+ind+abnf);
      	passed = false;
      	break;
      }
      else {
      	jsonstring = abnf.ToJSON();
      	json = JSON.Parse(jsonstring);
      	if (json.IsError()) {
      		out.println(ind+ind+json);
      		passed = false;
      		break;
      	}
      	tokens = (JSONObject)json;
      	out.println(ind+ind+tokens.GetSize()+" ABNF rules");
      	tokennames = tokens.GetMemberNames();
      	java.util.Arrays.sort(tokennames);
      	out.print(ind+ind+"Member names: ");
        for (i=0;i<tokens.GetSize();i++) {
        	if (i != 0)
        		out.print(",");
        	out.print(tokennames[i]);
        }
        out.println("");
      }

      passed = true;

      break;
    }		

    // Display result
		if (passed)
			out.println(ind+"Test PASSED");
		else
			out.println(ind+"Test FAILED");
		
		return;
  }

}
