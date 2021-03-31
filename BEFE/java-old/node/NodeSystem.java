//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// File:      NodeSystem.java - Local system as a hierarchy node
//
// Author:    bferris
//
// Copyright: 2011, Bruce Ferris
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

package bferris.node;

import bferris.core.Id;
import bferris.core.Ids;
import bferris.core.Factory;
import bferris.core.Object;
import bferris.core.Error;

import java.util.*;

//
// Imports
//

import java.io.File;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            NodeSystem - Local system as a hierarchy node
//
// Class Members:    ***None***
//
// Class Methods:    GetFullPath - Get OS dependent full path to node
//                   IsHidden    - Is a given node "hidden" by the OS?
//
// Instance Members: ***None***
//
// Instance Methods: Open              - "Open" the NodeSystem
//                   Close             - "Close" the NodeSystem
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

public class NodeSystem extends NodeBase implements Node, Factory {

	// 
	// Class members
	//
	
	public static final char Separator = '/';
	
  //
  // Class Methods
  //

  public static boolean IsHidden(Node node) {
	
  	File file;
  
  	file = new File(node.GetFullName());
  	return (file.isHidden() || !file.exists());
  }

  public static String[] SplitPath(String fullpath) {
    int index;
    int nextindex;
    List<String> parts;
    
    parts = new ArrayList<String>();
    
    index = 0;
    nextindex = 0;
    while (nextindex<fullpath.length()) {
      nextindex = fullpath.indexOf(Separator,nextindex);
    	if (nextindex < 0)
    		nextindex = fullpath.length();
    	if (nextindex-index > 0)
    		parts.add(fullpath.substring(index,nextindex));
    	index = ++nextindex;
    }
    
    return parts.toArray(new String[0]);
  }
  
  //
  // Instance Members
  //

  private boolean showhidden = false;
  
  //
  // Instance Methods
  //

  @Override
  public Object New(Id typeid) {
  	
    if (typeid == Ids.TypeSystem)
    	return new NodeSystem();
    return super.New(typeid);    	
  }
  
  public NodeSystem() {
    super(Ids.TypeSystem);
    if (getClass() == NodeSystem.class)
      Init();
  }

  protected NodeSystem(Id typeid) {
    super(typeid);
    if (getClass() == NodeSystem.class)
      Init();
  }

  private void Init() {
  	
    String os;

    os = System.getProperty("os.name");
    if (!os.equals("Windows Vista"))
      throw new RuntimeException("Operating system '"+os+"' not implemented yet");

    name = System.getenv("userdomain");
    if (name == null)
      name = "<Unknown>";
    
  }
  
  public String GetAddress() {
  	String sep = "" + Separator;
 		return sep + sep + GetRoot().GetName() + sep + GetFullName();
  }
  
  public boolean GetShowHidden() {
	return showhidden;
  }
  
  public void SetShowHidden(boolean show) {
	showhidden = show;
  }
  
  public void HideHidden() {
	showhidden = false;
  }
  
  public void RefreshChildren() {

  	Node           node;
    NodeSystemDisk disk;
    String         diskname;
    char           driveletter;

    synchronized (this) {

    	// Remove current children first
    	while ((NodeSystem)firstchild != null)
    		firstchild.SetParent(null);
    	
      for (driveletter='A'; driveletter <= 'Z'; driveletter++) {

    	  diskname = new Character(driveletter) + ":";
        node = FirstChildByName(diskname);
        if (node == null) {
          disk = new NodeSystemDisk(diskname);
          if (disk.Exists())
            AppendChild(new NodeSystemDisk(diskname));
        }
        else {
          if (node.getClass() != NodeSystemDisk.class)
              throw new RuntimeException("Internal error: Named child isn't a NodeSystemDisk");
          disk = (NodeSystemDisk)node;
          if (!disk.Exists()) {
            disk.SetParent(null);
          }
        }
      }
    }
    
    return;
  }

  @Override
  public Error SetName(String name) {
    return super.SetName(name);
  }

  @Override 
  public Error SetParent(Node node) {
  	return super.SetParent(node);
  }
  
  @Override
  public String GetFullName() {
  	Node[] path;
  	String fullpath;
  	
  	fullpath = "";
  	path = GetPath();
  	for (Node node : path) {
  		fullpath = fullpath + Separator + node.GetName();
  	}

  	return fullpath;
  	
  }
  
  @Override
  public Node FirstChildByName(String searchname) {
  	
  	Node curchild;
  	
  	searchname = searchname.toLowerCase();
  	synchronized (this) {
      for (curchild=GetFirstChild(); curchild != null; curchild = curchild.GetNextSibling()) {
        if (curchild.GetName().toLowerCase().equals(searchname))
  	  	  break;
  	  }
        
  	}
  	
  	return curchild;
    }

  @Override
  public Node FindChildByFullName(String fullpath) {
  	
  	NodeSystem child;
  	Node found;
  	String[] names;
  	
  	if (name == null)
  		return null;
  	
  	names = SplitPath(fullpath);
  	found = null;
  	child = this;
		RefreshChildren();
  	for (String name: names) {
  		child = (NodeSystem)child.FirstChildByName(name);
  		if (child == null)
  			break;
  	}
  	found = child;

  	return found;
  }
  
}
