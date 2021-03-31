
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
//File:      NodeSystemDisk.java - System Disk drive as a hierarchy node
//
//Author:    bferris
//
//Copyright: 2011, Bruce Ferris
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

package bferris.node;

import bferris.core.Id;
import bferris.core.Ids;
import bferris.core.Error;

//
//Imports
//

import java.io.File;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            NodeSystemDisk - Local System Disk drive as a hierarchy node
//
// Class Members:    ***None***
//
// Class Methods:    GetDirectoryContents
//
// Instance Members: ***None***
//
// Instance Methods: Exists
//                   RefreshChildren
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

public class NodeSystemDisk extends NodeSystem {

  //
  // Class Members
  //
  
  //
  // Class Methods
  //
  	
  public static NodeSystemDisk[] GetDirectoryContents(String fullpath) {
  
    NodeSystemDisk[]   nodes;
    File     dir;
    File     file;
    String[] names;
    int      i;
    
    dir = new File(fullpath);
    try {
    	names = dir.list();
  	  java.util.Arrays.sort(names);
    }
    catch (Exception e) {
  	  names = new String[0];
    }
  
    nodes = new NodeSystemDisk[names.length];
    i = 0;
    for (String name : names) {
  	  file = new File(dir,name);
  	  if (file.isDirectory())
  	    nodes[i] = new NodeSystemDirectory(name);
  	  else
  	    nodes[i] = new NodeSystemFile(name);
  	  i++;
    }
    
    return nodes;
  }

  //
  // Instance Members
  //

  //
  // Instance Methods
  //

  public NodeSystemDisk(Id typeid) {
  	super(typeid);
  }
  
  public NodeSystemDisk(String drivename) {
    super(Ids.TypeDisk);

    if (drivename.length() != 2)
      throw new RuntimeException("Expected two letter drive name, was given '"+drivename+"'");
    if (drivename.charAt(1) != ':')
      throw new RuntimeException("Expected two letter drive name ending in ':', was given '"+drivename+"'");

    name = drivename;
  }

  public boolean Exists() {
    File file;
    file = new File(GetFullName());
    return file.exists();
  }

  @Override
  public Error SetParent(Node node) {
  	return super.SetParent(node);
  }
  
  @Override
  public String GetFullName() {
  	
  	String fullpath;
  	Node   node;
  	
  	fullpath = GetName();
  	for (node=GetParent();node != null && node.GetParent() != null;node=node.GetParent())
  		fullpath = node.GetName() + File.separator+fullpath;
  	if (NodeSystemDisk.class.isInstance(this))
  		fullpath = fullpath + File.separator;
  	return fullpath;
  	
  }
  
  @Override
  public void RefreshChildren() {

    NodeSystemDisk[]  children;
    String            fullname;
    
    synchronized (this) {
    	
      // Clear current children
      while (firstchild != null)
      	firstchild.SetParent(null);

      // Populate new children if we have a parent
      if (parent != null) {
        fullname = GetFullName();
        children = GetDirectoryContents(fullname);
        for (Node child : children)
      	  AppendChild(child);
      }
    }
  }

  public Node FirstChildByName(String searchname) {
  	
  	
    RefreshChildren();
    return super.FirstChildByName(searchname);
  }

  @Override
  public Node FindChildByFullName(String fullpath) {
  	
    RefreshChildren();
    return super.FindChildByFullName(fullpath);
  }

}
