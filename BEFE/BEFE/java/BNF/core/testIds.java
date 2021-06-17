package bferris.core;

import java.io.PrintStream;

import bferris.node.*;

public class testIds {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test bferris.core.Ids
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    String      ind;
    PrintStream out;
    Object      obj;
    Node        node;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing Ids...");

    obj = Ids.New(Ids.TypeObject);
    if (obj == null)
    	out.println(ind+"obj              = null !!!Wrong");
    else
    	out.println(ind+"obj.GetTypeId()  = "+obj.GetTypeId().ToString());

    node = (Node)Ids.New(Ids.TypeNode);
    if (node == null)
    	out.println(ind+"node             = null !!!Wrong");
    else
    	out.println(ind+"node.GetTypeId() = "+node.GetTypeId().ToString());
  }

}
