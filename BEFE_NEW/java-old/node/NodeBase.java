package bferris.node;

import java.util.HashMap;
import java.util.Set;
import java.util.ArrayList;

import bferris.core.*;
import bferris.core.Object;
import bferris.core.Error;
import bferris.json.*;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            NodeBase - Base class for Node implementation
//
// Class Members:    ***None***
//
// Class Methods:    ***None***
//
// Instance Members: ***None***
//
// Instance Methods: Register          -
//                   Unregister        -
//                   Copy              -
//                   Delete            -
//
//                   ... Name and content methods...
//
//                   GetName            -
//                   SetName            -
//                   GetTitle           -
//                   SetTitle           -
//                   GetAttribute       -
//                   SetAttribute       -
//                   GetAttributeNames  - Get list of attribute names
//                   GetContent         = Get content
//                   SetContent         - Set content
//
//                   GetRoot           - Get root node in hierarchy
//                   PathToRoot        - Get List of nodes up to root node
//                   IsAncestorOf      - Is this Node ancestor of another Node?
//                   IsDescendentOf    - Is this Node a descendent of another Node?
//                   GetParent         - Get parent node
//                   SetParent         - Set parent node
//                   GetLength         - Get current number of children
//                   GetNextSibling    - Get next sibling node with same parent
//                   GetPrevSibling    - Get next sibling node with same parent
//                   GetChild          - Get Nth child
//                   GetFirstChild     - Get first child node
//                   GetLastChild      - Get last child node
//                   GetChildren       - Get list of all child nodes
//
//                   ... Structural manipulation methods...
//
//                   AppendChild       - Append child node
//                   PrependChild      - Prepend child node
//                   InsertBeforeChild - Insert child node before another child
//                   InsertAfterChild  - Insert child node after another child
//
//                   ... Query methods...
//
//                   FirstChildByName    - Find first child with given name
//                   NextSiblingByName   - Find next sibling with same name
//                   FindChildByNamePath - Find child given name path
//
// This class implements a general purpose organisational hierarchy node.
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

public class NodeBase extends bferris.core.Object implements Node, Factory {

  //
  // Class Members
  //

  protected static Id classtypeid = Ids.TypeNode;
  
  //
  // Class Methods
  //

  @Override
  public Object New() {
	  return new NodeBase();
  }
  
  //
  // Instance Members
  //

  protected Node                   parent      = null;
  protected Node                   firstchild  = null;
  protected Node                   prevsibling = null;
  protected Node                   nextsibling = null;

  protected String                 name        = null;
  protected String                 title       = null;
  protected HashMap<String,String> attributes  = null;
  protected Object                 content     = null;

  //
  // Instance Methods
  //

  public NodeBase() {
    super(Ids.TypeNode);
  }

  public NodeBase(Id typeid) {
    super(typeid);
  }

  public NodeBase(Id id, Id typeid) {
    super(id,typeid);
  }

  // *** Playing about here ***
  @Override
  public JSON ToJSON(JSON out) {
  	
  	JSONObject json;

  	out = super.ToJSON(out);
  	json = (JSONObject)out;
  	
  	// Serialise this...
  	
  	if (parent != null)
    	json.SetMember(new JSONMember("parent",new JSONString(parent.GetId().ToString())));
  	if (firstchild != null)
    	json.SetMember(new JSONMember("firstchild",new JSONString(firstchild.GetId().ToString())));
  	if (prevsibling != null)
    	json.SetMember(new JSONMember("prevsibling",new JSONString(prevsibling.GetId().ToString())));
  	if (nextsibling != null)
    	json.SetMember(new JSONMember("nextsibling",new JSONString(nextsibling.GetId().ToString())));

  	if (name != null)
    	json.SetMember(new JSONMember("name",new JSONString(name)));
  	if (title != null)
    	json.SetMember(new JSONMember("title",new JSONString(name)));

  	return json;
  }
  
  public Repository GetRepository() {
  	return Object.GetObjectRepository();
  }
  
  public String GetAddress() {
  	return GetId().ToString();
  }
  
  public String GetName() {
    if (name == null)
      return "";
    return new String(name);
  }

  public Error SetName(String name) {
  	if (name != null)
      this.name = new String(name);
  	else
  		this.name = "";
  	return null;
  }

  public String GetTitle() {
    if (title == null)
      return GetName();
    return new String(title);
  }

  public Error SetTitle(String title) {
  	if (title != null)
      this.title = new String(title);
  	else
  		this.title = null;
  	return null;
  }

  public String GetAttribute(String name) {
    if (attributes == null || !attributes.containsKey(name))
      return null;
    return attributes.get(name);
  }

  public synchronized Error SetAttribute(String name,String value) {

    if (attributes == null)
      attributes = new HashMap<String,String>();
    if (value != null)
      attributes.put(name,value);
    else {
      if (name != null && attributes.containsKey(name)) {
          attributes.remove(name);
          if (attributes.isEmpty())
            attributes = null;
      }
    }
    
    return (Error)null;
  }

  public synchronized String[] GetAttributeNames() {

    Set<String> set;
    String[]    slist;

    if (attributes == null)
      set = new HashMap<String,Object>().keySet();
    else
      set = attributes.keySet();
    slist = (String[])set.toArray();

    return slist;
  }

  public Node GetRoot() {

  	Node            root;
  	NodeBase        that;
  	ArrayList<Node> path;
  	
  	path = new ArrayList<Node>();
  	
  	root = null;
  	synchronized (this.getClass()) {
  		that = this;
  		while (that.parent != null) {
    		path.add(that);
  			that = (NodeBase)that.parent;
  			if (path.contains(that))
  				throw new RuntimeException("Circular parent chain");
  		}
  		root = that;
  	}
    return root;
  }

  public Node[] GetPath() {

    Node[] path = null;
    Node node;

    synchronized (this.getClass()) {

      int depth;

      node = this;
      for (depth=1; node.GetParent() != null; depth++)
        node = node.GetParent();
      path = new Node[depth];
      node = this;
      depth = path.length;
      do {
        path[--depth] = node;
        node = node.GetParent();
      }
      while (node != null);

    }
    
    return path;
  }

  public String GetFullName() {
  	if (GetId() == null)
  		return "";
  	else
    	return GetId().ToString();
  }
  
  public boolean IsAncestorOf(Node that) {
    Node node;

    synchronized (this.getClass()) {
      node = that;
      while (node != null) {
        node = node.GetParent();
        if (node.GetId().Equals(GetId()))
          return true;
      }
    }
    
    return false;
  }

  public boolean IsDescendentOf(Node that) {
  	Id   thatId;
    Node node;

    if (that == null)
    	return false;
    
    thatId = that.GetId();
    synchronized (this.getClass()) {
      node = this;
      while (node != null) {
        node = node.GetParent();
        if (node.GetId().Equals(thatId))
          return true;
      }
    }
    
    return false;
  }

  public Node GetParent() {
    return parent;
  }

  @Override
  public Error SetParent(Node parent) {

  	Error     result;
  	NodeBase  oldparent;
  	NodeBase  newparent;
  	Node      newroot;
  	
  	result = null;
  	
    processing:	do {
  	
  	  // Sanity checks
  	  if (repos == null) {
  	  	result = new Error("No Root Repository");
  	  	break processing;
  	  }
  	  oldparent = null;
  	  newparent = null;
  	  if (parent != null)
        newparent = (NodeBase)repos.GetNode(parent.GetId());
  	  else
  	  	newparent = null;
      if (newparent != null) {
        newroot = newparent.GetRoot();
        if (newroot == null || !newroot.GetId().Equals(repos.GetId())) {
        	result = new Error("Cannot set parent to a different repository");
        	break processing;
        }
      }
      
  	  // Remove from current parent if any and not same...
      if (this.parent != null && this.parent != newparent) synchronized (repos) {

      	oldparent = (NodeBase)repos.GetNode(this.parent.GetId());
      	if (oldparent == null || oldparent.GetId() == null) {
      		result = new Error("Current parent "+this.parent.GetId()+" dead or missing");
      		break processing;
      	}
    	  if (prevsibling != null && prevsibling.GetId() == null) {
    	  	result = new Error("Previous sibling "+prevsibling.GetId()+" dead or missing");
    	  	break processing;
    	  }
    	  if (nextsibling != null && nextsibling.GetId() == null) {
    	  	result = new Error("Next sibling "+nextsibling.GetId()+" dead or missing");
    	  	break processing;
    	  }
    	
        if (oldparent.firstchild == this)
        	oldparent.firstchild = nextsibling;
        if (prevsibling != null)
          ((NodeBase)prevsibling).nextsibling = nextsibling;
        if (nextsibling != null)
          ((NodeBase)nextsibling).prevsibling = prevsibling;
        this.parent      = null;
        this.nextsibling = null;
        this.prevsibling = null;
      }

      // Add to end of new parent's list of children if parent set...
      if (newparent != null)
        result = newparent.AppendChild(this);
    
    } while (false);
  	
    return result;

  }

  public Node GetNextSibling() {

    return nextsibling;

  }

  public Node GetPrevSibling() {

    return prevsibling;

  }

  public int GetLength() {

    Node child;
    int  count;

    count = 0;

    synchronized (repos) {

      child = GetFirstChild();
      while (child != null) {
        count++;
        child = child.GetNextSibling();
      }

    }

    return count;

  }

  public synchronized Node GetChild(int index) {

    Node child;
    int  childno;
    int  len;

    child = null;
    
    processing: do {
  	
      len = GetLength();
      if (index < 0)
        index = len + index;
      if (index < 0 || index >= len)
        child = null;
      else {
        child = GetFirstChild();
        for (childno=0; childno < index; childno++) {
        	child = repos.GetNode(child.GetId());
      	if (child == null)
      		break processing;
        child = child.GetNextSibling();
      }
    }
    
    break processing;
    
    } while (false);
    
    return child;

  }

  public Node GetFirstChild() {
    return firstchild;
  }

  public Node GetLastChild() {

    Node child;

    synchronized (GetRoot()) {

      child = GetFirstChild();
      while (child != null) {
        if (child.GetNextSibling() == null)
          break;
        child = child.GetNextSibling();
      }
    }

    return child;
  }

  public synchronized Node[] GetChildren() {

    Node[] children;
    Node   child;
    int    len;
    int    childno;

    synchronized (GetRoot()) {

      len = GetLength();
      children = new Node[len];
      child = GetFirstChild();
      for (childno=0; childno < len; childno++) {
        children[childno] = child;
        child = child.GetNextSibling();
      }
    }

    return children;
  }

  public synchronized Error AppendChild(Node child) {

    NodeBase lastchild;
    NodeBase nbchild;
    
    synchronized (GetRoot()) {

    	if (child == null)
    		throw new RuntimeException("Child?  What child?  null child passed");
      if (child.GetParent() != null)
        throw new RuntimeException("Cannot Append parented child, use SetParent(null) first");
      if (!getClass().isInstance(child))
      	throw new RuntimeException("Can only Append children of NodeBase class");
      nbchild = (NodeBase)child;
      
      lastchild = (NodeBase)GetLastChild();
      if (lastchild == null) {
        firstchild = nbchild;
      }
      else {
        lastchild.nextsibling          = nbchild;
        ((NodeBase)child).prevsibling  = lastchild;
      }
      nbchild.parent          = this;
    }

    return null;
  }

  public synchronized Error PrependChild(Node child) {

  	NodeBase nbchild;
  	
    synchronized (GetRoot()) {

      if (child.GetParent() != null)
        throw new RuntimeException("Cannot Prepend parented child, use SetParent(null) first");
      if (!getClass().isInstance(child))
      	throw new RuntimeException("Can only Prepend children of NodeBase class");
      nbchild = (NodeBase)child;
      
      if (firstchild == null) {
        firstchild = nbchild;
      }
      else {
        nbchild.nextsibling = firstchild;
        ((NodeBase)firstchild).prevsibling = nbchild;
        firstchild = nbchild;
      }
    }

    return null;
  }

  public synchronized Error InsertBeforeChild(Node child,Node sibling) {

    NodeBase nbchild;
    NodeBase nbsibling;
    NodeBase othersibling;

    synchronized (GetRoot()) {

      if (child.GetParent() != null)
        throw new RuntimeException("Cannot insert parented child, use SetParent(null) first");
      if (!getClass().isInstance(child))
      	throw new RuntimeException("Cannot insert child of non-NodeBase class");
      nbchild= (NodeBase)child;

      if (sibling == null)
        throw new RuntimeException("Sibling?  What sibling?  Null sibling specified");
      if (sibling.GetParent() != this)
      	throw new RuntimeException("Can only Insert before Node's own children");
      nbsibling = (NodeBase)sibling;

      othersibling = (NodeBase)firstchild;
      while (othersibling != null && othersibling != sibling)
        othersibling = (NodeBase)othersibling.nextsibling;
      if (othersibling != sibling)
        throw new RuntimeException("Something's wrong! Can't find sibling even though this is their parent");

      if (nbsibling.prevsibling != null) {
        ((NodeBase)(nbsibling.prevsibling)).nextsibling = nbchild;
        nbchild.prevsibling = nbsibling.prevsibling;
        nbchild.nextsibling = nbsibling;
        nbsibling.prevsibling = nbchild;
        nbchild.parent      = this;
      }
      else {
        nbsibling.prevsibling = nbchild;
        nbchild.nextsibling = nbsibling;
        firstchild = nbchild;
      }
    }

    return null;
  }

  public synchronized Error InsertAfterChild(Node child,Node sibling) {

    NodeBase nbchild;
    NodeBase nbsibling;
    NodeBase othersibling;

    synchronized (GetRoot()) {

      if (child.GetParent() != null)
        throw new RuntimeException("Cannot insert parented child, use SetParent(null) first");
      if (!getClass().isInstance(child))
      	throw new RuntimeException("Cannot insert child of non-NodeBase class");
      nbchild= (NodeBase)child;

      if (sibling == null)
        throw new RuntimeException("Sibling?  What sibling?  Null sibling specified");
      if (sibling.GetParent() != this)
      	throw new RuntimeException("Can only Insert before Node's own children");
      nbsibling = (NodeBase)sibling;

      othersibling = (NodeBase)firstchild;
      while (othersibling != null && othersibling != sibling)
        othersibling = (NodeBase)((NodeBase)othersibling).nextsibling;
      if (othersibling != sibling)
        throw new RuntimeException("Something's wrong! Can't find sibling even though this is their parent");

      if (nbsibling.nextsibling != null) {
        ((NodeBase)(nbsibling.nextsibling)).prevsibling = nbchild;
        nbchild.nextsibling = nbsibling.nextsibling;
        nbchild.prevsibling = nbsibling;
        nbsibling.nextsibling = nbchild;
        nbchild.parent      = this;
      }
      else {
        nbsibling.nextsibling = nbchild;
        nbchild.prevsibling = nbsibling;
        firstchild = nbchild;
      }
    }

    return null;
  }
  
  public synchronized Node FirstChildByName(String searchname) {
  	
  	Node curchild;
  	
  	synchronized (GetRoot()) {
 
      for (curchild=GetFirstChild(); curchild != null; curchild = curchild.GetNextSibling()) {
        if (curchild.GetName().equals(searchname))
  	  	  break;
  	  }
        
  	}
  	
  	return curchild;
    }

    public synchronized Node NextSiblingByName() {

  	Node cursibling;
  	
  	synchronized (GetRoot()) {
  		  
  	  for (cursibling=this; cursibling != null; cursibling = cursibling.GetNextSibling()){
  	    if (cursibling.GetName().equals(name))
  		  break;
  	  }

  	}
  	  
      return cursibling;
    }
    
    public Node FindChildByFullName(String fullname) {
    	return null;
    }
    
}
