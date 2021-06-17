package bferris.node;

public class NodePath extends NodeBase {

	private Node root;
	private Node curnode;
	
	public NodePath(Node root) {
		if (root == null)
			throw new RuntimeException("Expected non-null root");
		this.root = root;
		this.curnode = root;
	}
	
	public Node Down() {
		Node n;
		n = curnode.GetFirstChild();
		if (n != null)
			curnode = n;
		return n;
	}

	public Node Up() {
		Node n;
		n = curnode.GetParent();
		if (n != null) {
			curnode = n;
		}
		return n;
	}
	
	public Node Next() {
		Node n;
		n = curnode.GetNextSibling();
		if (n != null && n != root)
			curnode = n;
		return n;		
	}

	public Node Prev() {
		Node n;
		n = curnode.GetPrevSibling();
		if (n != null && n != root)
			curnode = n;
		return n;		
	}
	
	public Node Current() {
		return curnode;
	}

	public int Depth() {
		int depth;
		Node n;
		depth = 1;
		for (n=curnode;n!=root;n=n.GetParent())
		  depth++;
		return depth;
	}
	
}
