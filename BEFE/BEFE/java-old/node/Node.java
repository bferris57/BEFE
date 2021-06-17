package bferris.node;

import bferris.core.Error;
import bferris.core.Id;
import bferris.core.Repository;

abstract public interface Node {

	// Identity and ownership
	abstract public String     GetAddress();
	abstract public Id         GetId();
	abstract public Id         GetTypeId();
	abstract public Repository GetRepository();

	// Name, title, and attributes
  abstract public String   GetName();
  abstract public Error    SetName(String name);
  abstract public String   GetTitle();
  abstract public Error    SetTitle(String title);
  abstract public String   GetAttribute(String name);
  abstract public Error    SetAttribute(String name,String value);
  abstract public String[] GetAttributeNames();

  // Structure
  abstract public Node     GetRoot();
  abstract public Node[]   GetPath(); 
  abstract public String   GetFullName();
  abstract public boolean  IsAncestorOf(Node that);
  abstract public boolean  IsDescendentOf(Node that);
  abstract public Node     GetParent();
  abstract public Error    SetParent(Node parent);

  abstract public Node     GetNextSibling();
  abstract public Node     GetPrevSibling();
  abstract public int      GetLength();
  
  abstract public Node     GetChild(int index);
  abstract public Node     GetFirstChild();
  abstract public Node     GetLastChild();
  abstract public Node[]   GetChildren();
  abstract public Error    AppendChild(Node child);
  abstract public Error    PrependChild(Node child);
  abstract public Error    InsertBeforeChild(Node child,Node sibling);
  abstract public Error    InsertAfterChild(Node child,Node sibling);
  
  abstract public Node     FirstChildByName(String searchname);
  abstract public Node     NextSiblingByName();
  abstract public Node     FindChildByFullName(String fullpath);
  
}