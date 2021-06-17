package bferris.node;

import java.io.PrintStream;

public class testNodeSystem {

  //----------------------------------------------------------------------------
  //
  // Method: test_NodeSystem - Test NodeSystem class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    String         ind;
    PrintStream    out;
    NodeSystem     sys;
    Node           disk;
    Node           d;
    Node           n;
    String         drivename;
    
    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing NodeSystem...");
    
    sys = new NodeSystem();
      out.println(ind+"System: '"+sys.GetName()+"'");
    disk = sys.GetFirstChild();
    while (disk != null) {
      out.println(ind+"Disk  : '"+disk.GetName()+"'");
      disk = disk.GetNextSibling();
    }

    drivename = "D:";
    d = (NodeSystem)sys.FirstChildByName(drivename);
    if (d == null)
      out.println(ind+drivename+" not found!");
    else {
      n = d.GetFirstChild();
      while (n!=null) {
    	if (!NodeSystem.IsHidden(n))
          out.println(ind+n.GetFullName());
    	n = n.GetNextSibling();
      }
    }
  }


}
