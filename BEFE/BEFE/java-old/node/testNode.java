package bferris.node;

import java.io.PrintStream;
import bferris.core.Ids;

public class testNode {

  //----------------------------------------------------------------------------
  //
  // Method: testNode - Test Node class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}
	
  public static void test(String indent) {

    String      ind;
    PrintStream out;
    NodeBase    n1    = new NodeBase();
    NodeBase    n1p1  = new NodeBase();
    NodeBase    n1p2  = new NodeBase();
    NodeBase    n2p1  = new NodeBase();
    NodeBase    n2p2  = new NodeBase();
    Node[]      nodes = {n1,n1p1,n1p2,n2p1,n2p2};
    Node[]      children;
    Node[]      path;
    String      pathstr;
    int         i;
    int         j;

    ind = indent + "  ";
    out = System.out;

    out.println(indent+"***Testing Node...");

    {
    	NodeBase trythis;
    	trythis = (NodeBase)Ids.New(Ids.TypeNode);
    	out.println(ind+"trythis             = "+trythis.toString());
    	out.println(ind+"trythis.GetTypeId() = "+trythis.GetTypeId().toString());
    }
    n1.SetName("Node 1");
    n1p1.SetName("Node 1.1");
    n1p2.SetName("Node 1.2");
    n2p1.SetName("Node 1.2.1");
    n2p2.SetName("Node 1.2.2");
    n1.AppendChild(n1p1);
    n1.AppendChild(n1p2);
    n1p2.AppendChild(n2p1);
    n1p2.AppendChild(n2p2);

    for (i=0;i<nodes.length;i++) {
      out.println(ind+"Node "+i+": name = '"+nodes[i].GetName()+"'");
    }

    children = n1p2.GetChildren();
    for (i=0;i<children.length;i++) {
      out.println(ind+"Node "+n1p2.GetName()+": Child "+i+" = "+children[i].GetName());
    }

    for (i=0;i<nodes.length;i++) {
      path = nodes[i].GetPath();
      pathstr = "";
      for (j=0;j<path.length;j++) {
        if (j != 0)
          pathstr = pathstr + " -> ";
        pathstr = pathstr + path[j].GetName();
      }
      out.println(ind+"Path from Node "+String.format("%10s",nodes[i].GetName())+" to root: "+pathstr);
    }

    out.println(ind+"");
    out.println(ind+(n1p2.IsDescendentOf(n1)?"'Node 1.2' IS a descendent of 'Node 1'":"'Node 1.2' is NOT a descendent of 'Node 1'"));
    out.println(ind+(n1.IsAncestorOf(n1p2)?"'Node 1' IS an ancestor of 'Node 1.2'":"'Node 1' is NOT an ancestor of 'Node 1.2'"));
    out.println(ind+"'Node 1' GetFullName() = "+n1.GetFullName());
    out.println(ind+"'Node 1' ToJSON()      = "+n1.ToJSON());
    
  }

}
