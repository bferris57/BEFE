package bferris.core;

import java.io.PrintStream;

import bferris.json.*;
import bferris.node.NodeBase;

public class testRepository {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test Id class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}

	public static boolean IsError(Object that) {
		return Object.IsError(that);
	}
	
	public static Repository NewTestRepository(String name) {

		Repository repos;
		
		repos = new Repository(name);
		
    NodeBase    n1    = new NodeBase();
    NodeBase    n1p1  = new NodeBase();
    NodeBase    n1p2  = new NodeBase();
    NodeBase    n2p1  = new NodeBase();
    NodeBase    n2p2  = new NodeBase();
    //Node[]      nodes = {n1,n1p1,n1p2,n2p1,n2p2};
    n1.SetName("Node 1");
    n1p1.SetName("Node 1.1");
    n1p2.SetName("Node 1.2");
    n2p1.SetName("Node 1.2.1");
    n2p2.SetName("Node 1.2.2");
    n1.AppendChild(n1p1);
    n1.AppendChild(n1p2);
    n1p2.AppendChild(n2p1);
    n1p2.AppendChild(n2p2);

    Object.SetRepository(null);
    
    return repos;
    
	}

	public static void testJSON(String indent) {

		String      ind;
    PrintStream out;
    
    Repository  repos;
    JSON        json;
    String[]    result;
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(ind+"ToJSON()...");
    repos = NewTestRepository("Test Repository");
    Object.SetRepository(repos);
    
    json = repos.ToJSON();
    out.println(ind+ind+"JSON = ...");
    result = json.ToStringArray();
    for (String s : result)
    	out.println(ind+ind+ind+s);
  
  }

	public static void testSave(String indent) {

		String      ind;
    PrintStream out;
    
    Repository  repos;
    Error       result;
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(indent+"Save...");
    repos = NewTestRepository("Test Repository");
    result = repos.SaveTo(System.getProperty("java.io.tmpdir")+"bferris\\testrepository.json");
    if (IsError(result))
    	out.println(ind+result);
    else
    	out.println(ind+"OK");
  }
	
  public static void test(String indent) {

    PrintStream out;
    String      ind;
    
    out = System.out;
    ind = indent+"  ";
    
    out.println(indent+"***Testing Repository...");

    //testJSON(ind);
    testSave(ind);
    
  }

}
