package bferris.node;

import java.io.PrintStream;

public class testNodePath {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test NodePath class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

  	String      ind;
    PrintStream out;
    Node        n1    = new NodeBase();
    Node        n1p1  = new NodeBase();
    Node        n1p2  = new NodeBase();
    Node        n2p1  = new NodeBase();
    Node        n2p2  = new NodeBase();
    NodePath    path;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing NodePath...");

    //
    // Test basic path walking
    //
    
    // Make the nodes
    n1.SetName("Node 1");
    n1p1.SetName("Node 1.1");
    n1p2.SetName("Node 1.2");
    n2p1.SetName("Node 1.2.1");
    n2p2.SetName("Node 1.2.2");
    n1.AppendChild(n1p1);
    n1.AppendChild(n1p2);
    n1p2.AppendChild(n2p1);
    n1p2.AppendChild(n2p2);

    // Make the NodePath
    path = new NodePath(n1);

    // Walk the path displaying leaf nodes...
    out.println(ind+"Leaf walking...");
    while (true) {
    	// Walk until at bottom
    	while (path.Down() != null);
    	// Print the depth and node name
    	out.println(ind+ind+path.Depth()+": '"+path.Current().GetName().toString()+"'");
    	// Try next one...
    	if (path.Next() != null)
    		continue;
      // Go up
    	while (path.Up() != null) {
    		if (path.Next() != null)
    			break;
    	}
    	if (path.Depth() == 1)
    		break;
    }
    
    //
    // Test searching for named file
    //
    
    // Search for named file
    String[]    files = {"d:\\cloud\\dropbox\\svn\\bferris",
    		                 "d:\\cloud\\dropbox\\.mime.types",
    		                 "d:\\cloud\\dropbox\\.mimetypes",
    		                 "d:\\cloud\\dropbox\\svn\\",
                         "d:\\cloud\\dropbox\\svn",
                         "d:\\cloud\\dropbox\\",
                         "d:\\cloud\\dropbox",
                     		 "d:\\cloud",
    		                 "d:\\",
    		                 "d:",
    		                 "x:\\"
                        };
    String      fname;
    NodeSystem  sys;
    int         fileno;

    out.println(ind+"Native file names...");
    sys = new NodeSystem();
    for (fileno=0;fileno<files.length;fileno++) {
    	Node child;
      fname = files[fileno];
      child = sys.FindChildByFullName(fname);
      if (child == null)
      	out.println(ind+ind+"File '"+fname+"' not found");
      else {
      	out.println(ind+ind+"File '"+fname+"' found");
        if (child.getClass() == NodeSystemFile.class)
        	out.println(ind+ind+"  mime type = "+((NodeSystemFile)child).GetMimeType());
        out.println(ind+ind+"  address   = "+child.GetAddress());
      }
    }    
  }

}
