package bferris.json;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.ArrayList;

import bferris.json.JSON.JSONType;
import bferris.core.Error;
import bferris.util.*;

public class testJSON {

	//----------------------------------------------------------------------------
  //
  // Method: test_JSON - Test JSON... classes
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}

	public static void PrintStats(String ind, String header, JSON json) {

		PrintStream out = System.out;

		// Print header if anything specified
		if (header != null) {
			out.println(ind+header);
			out.println();
		}
		
		// Calculate totals
		JSON.Analysis stats;
		stats = new JSON.Analysis(json);
		out.println(ind+"Total:    Type JSON     = " + stats.total[JSONType.JSON.ordinal()]); 
		out.println(ind+"               Error    = " + stats.total[JSONType.Error.ordinal()]); 
		out.println(ind+"               Object   = " + stats.total[JSONType.Object.ordinal()]); 
		out.println(ind+"               Member   = " + stats.total[JSONType.Member.ordinal()]); 
		out.println(ind+"               Array    = " + stats.total[JSONType.Array.ordinal()]); 
		out.println(ind+"               Element  = " + stats.total[JSONType.Element.ordinal()]); 
		out.println(ind+"               Null     = " + stats.total[JSONType.Null.ordinal()]); 
		out.println(ind+"               True     = " + stats.total[JSONType.True.ordinal()]); 
		out.println(ind+"               False    = " + stats.total[JSONType.False.ordinal()]); 
		out.println(ind+"               String   = " + stats.total[JSONType.String.ordinal()]); 
		out.println(ind+"               Number   = " + stats.total[JSONType.Number.ordinal()]);
		out.println();
		out.println(ind+"Array:  Total  Elements = " + stats.totalArrayElements);
		out.println(ind+"        Mean   Elements = " + stats.meanArrayElements);
		out.println(ind+"        Max    Elements = " + stats.maxArrayElements);
		out.println();
		out.println(ind+"Object: Total  Members  = " + stats.totalObjectMembers);
		out.println(ind+"        Mean   Members  = " + stats.meanObjectMembers);
		out.println(ind+"        Max    Members  = " + stats.maxObjectMembers);
		out.println();
		out.println(ind+"Max     Depth           = " + stats.maxDepth);

	}
	
	public static void testParsing(String indent) {
		
  	String      ind;
    PrintStream out;
    JSON        json1;
    JSON        json2;

    ind = indent + "  ";
    out = System.out;

    out.println(indent+"Testing JSON Parsing...");
    
    // Test backup of backup .json file...
		String fname;
		File   jfile;
		String jstring1;
		String jstring2;
		
    fname = "d:\\cloud\\dropbox\\SVN\\bferris\\bferris\\testfiles\\testfirefox.json";
    //fname = "d:\\cloud\\dropbox\\SVN\\bferris\\bferris\\testfiles\\test.json";
    jfile = new File(fname);
    if (!jfile.exists())
    	throw new RuntimeException("File '"+fname+"' does not exist");
    jstring1 = ReadFile.ReadString(jfile);
		json1 = JSON.Parse(jstring1);
		if (json1.IsError())
  		out.println(ind+"Parsing failed: "+json1);
		else
			out.println(ind+"Parsing OK");
		jstring2 = json1.ToJSON();
		if (jstring1.length() != jstring2.length()) {
			out.println(ind+"Input json and output json are different lengths");
			out.println(ind+"  Input length = "+jstring1.length()+" Output length = "+jstring2.length());
		}
		json2 = JSON.Parse(jstring2);
		if (json2.IsError()) {
			out.println(ind+"Input json produced bad JSON text output: "+json2);
		}
		else {
			if (json1.IsEquivalentTo(json2))
				out.println(ind+"Output JSON is equivalent to Input JSON");
      else
				out.println(ind+"Output JSON is NOT equivalent to Input JSON");
		}

    // PrintStats(ind,"*** Parsing Statistics ***",json1);
    

  }
	
	public static void testEquivalency(String indent) {

  	String      ind;
    PrintStream out;
    JSON        json1;
    JSON        json2;

    ind = indent + "  ";
    out = System.out;

    out.println(indent+"Testing JSON Equivalency...");
    
    json1 = new JSONFalse();
    json2 = new JSONFalse();
    out.println(ind+"JSONFalse()           with JSONFalse()           = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONFalse();
    json2 = new JSONTrue();
    out.println(ind+"JSONFalse()           with JSONTrue()            = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.NullObject;
    json2 = null;
    out.println(ind+"JSON.NullObject       with null                  = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.NullObject;
    json2 = json1;
    out.println(ind+"JSON.False()          with itself                = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONNumber(1);
    json2 = new JSONNumber(1);
    out.println(ind+"JSONNumber(1)         with JSONNumber(1)         = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONNumber(1);
    json2 = new JSONNumber(2);
    out.println(ind+"JSONNumber(1)         with JSONNumber(2)         = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONNumber(1);
    json2 = JSON.Parse("1");
    out.println(ind+"JSONNumber(1)         with JSON.Parse(\"1\")       = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONString("1");
    json2 = new JSONString("2");
    out.println(ind+"JSONString(\"1\")       with JSONString(\"2\")       = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONString("1");
    json2 = json1;
    out.println(ind+"JSONString(\"1\")       with itself                = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = new JSONString("1");
    json2 = new JSONNumber(1);
    out.println(ind+"JSONString(\"1\")       with JSONNumber(1)         = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("[]");
    json2 = JSON.Parse("[]");
    out.println(ind+"[]                    with []                    = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("[1,2,3]");
    json2 = JSON.Parse("[1,2,3]");
    out.println(ind+"[1,2,3]               with [1,2,3]               = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("[1,2,3]");
    json2 = JSON.Parse("[3,2,1]");
    out.println(ind+"[1,2,3]               with [3,2,1]               = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("{}");
    json2 = JSON.Parse("{}");
    out.println(ind+"{}                    with {}                    = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("{\"id\":1}");
    json2 = JSON.Parse("{\"id\":1}");
    out.println(ind+"{\"id\":1}              with {\"id\":1}              = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("{\"id\":1}");
    json2 = JSON.Parse("{\"id\":2}");
    out.println(ind+"{\"id\":1}              with {\"id\":2}              = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("{\"id\":1,\"name\":\"foo\"}");
    json2 = JSON.Parse("{\"id\":1,\"name\":\"foo\"}");
    out.println(ind+"{\"id\":1}              with {\"id\":1,\"name\":\"foo\"} = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    json1 = JSON.Parse("{\"id\":1,\"name\":\"foo\"}");
    json2 = JSON.Parse("{\"id\":1,\"name\":\"foo\"}");
    out.println(ind+"{\"id\":1,\"name\":\"foo\"} with {\"id\":1,\"name\":\"foo\"} = "+((json1.IsEquivalentTo(json2))?"true":"false"));
    
	}

  private static JSONMember NewTestMember(String name, String value) {
  	JSONMember member = new JSONMember();
    JSONString val;
  	member.SetName(new JSONString('"'+name+'"'));
  	val = new JSONString('"'+value+'"');
  	val.Finalise();
  	member.SetValue(val);
  	member.Finalise();
  	return member;
  }
  
	public static void testJSONObject(String indent) {
		
  	String                ind;
    PrintStream           out;
    JSONObject[]          objects = {JSONObject.New(), JSONObject.NewOrdered()};
    JSONObject            obj;
    int                   objidx;
    String[]              titles  = {"Unordered",      "Ordered"};
    ArrayList<JSONMember> members;
    JSONMember            member;
    int                   membidx;
    JSON                  json;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"Testing JSONObject...");
    
    // Build the test members
    members = new ArrayList<JSONMember>();
    members.add(NewTestMember("id","1"));
    members.add(NewTestMember("title","hi there"));
    members.add(NewTestMember("xxx","yyy"));
    members.add(NewTestMember("id","2"));
    
    // Test each one...
    for (objidx=0; objidx < objects.length; objidx++) {
    	out.println(ind+"---"+titles[objidx]+"---");
    	obj = objects[objidx];
      for (membidx=0; membidx < members.size(); membidx++) {
      	member = members.get(membidx);
      	out.println(ind+"Adding "+member.ToJSON()+"...");
        json = obj.SetMember(member);
        if (json.IsError())
        out.println(ind+"  "+json.toString());
      }
      for (membidx=0; membidx < members.size(); membidx++) {
      	member = members.get(membidx);
      	out.println(ind+"Searching for "+member.GetName().ToJSON()+"...");
      	json = obj.FindByName(member.GetName().toString());
      	out.println(ind+"  Found: "+json.ToJSON()+"");
      }
      out.println(ind+"JSON = "+obj.ToJSON());
    }
	}
	
	public static void testBeautify(String indent) {
		
  	String      ind;
    PrintStream out;
		String      fname;
		File        jfile;
		String      jstring;
		JSON        json;
		String[]    jstrings;
		Error       result;
    PrintStream ofile;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"Testing JSON Beautify...");
    

		result = null;

  	test:	do {
			
			// Read the original FireFox bookmark json...
      fname = "d:\\cloud\\dropbox\\SVN\\bferris\\bferris\\testfiles\\testfirefox.json";
      jfile = new File(fname);
      if (!jfile.exists()) {
      	result = new Error("File '"+fname+"' does not exist");
      	break test;
      }
      jstring = ReadFile.ReadString(jfile);
	  	json = JSON.Parse(jstring);
	  	if (json.IsError()) {
	  		result = new Error("Error: "+json.ToString());
	  		break test;
	  	}
	  	
	  	// Convert to String[] and write it
	  	jstrings = json.ToStringArray();

      // Create the directories if not already there...
      fname = System.getProperty("java.io.tmpdir")+"bferris\\testfirefox.json";
      jfile = new File(fname);
  		try {jfile.getParentFile().mkdirs();}
  		catch (Throwable e) {
  			result = new Error("Failed to create parent directories for '"+fname+"'");
  			break test;
  		}
  		
  	  // Open the file
  	  try {
    	  ofile = new PrintStream(new FileOutputStream(jfile));
  	  }
  	  catch (FileNotFoundException e) {
  	  	result = new Error("Failed to create output file '"+fname+"'");
  	  	break test;
  	  }

      // Write the lines and close the file
  	  for (String line: jstrings)
  	    ofile.println(line);
  	  ofile.close();
      out.println(ind+jstrings.length+" JSON lines output");

		} while (false);
		
		if (result != null)
      out.println(ind+result);
		else
			out.println(ind+"OK");

  }
	
  public static void test(String indent) {

    PrintStream out;

    out = System.out;

    out.println(indent+"***Testing JSON...");
    
    //testParsing(indent);
    //testEquivalency(indent);
		//testJSONObject(indent);
    testBeautify(indent);
		out.println("");
  }

}
