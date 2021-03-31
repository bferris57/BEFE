//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// File:      NodeSystemDirectory.java - <Description>
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
import bferris.core.Error;

//
// Imports
//


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            NodeSystemDirectory - A raw system directory (e.g. binary bytes)
//
// Class Members:
//
// Class Methods:
//
// Instance Members:
//
// Instance Methods:
//
// <Notes>
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

public class NodeSystemDirectory extends NodeSystemDisk {

  //
  // Class Members
  //

  //
  // Class Methods
  //

  //
  // Instance Members
  //

  protected String src;
  protected String fullpath;

  //
  // Instance Methods
  //

  public NodeSystemDirectory(String name) {
    super(Ids.TypeDirectory);
    this.name = name;
    Init();
  }

  public NodeSystemDirectory(Id typeid) {
  	super(typeid);
  	Init();
  }
  
  private void Init() {
    src      = "";
    fullpath = "";
    //RefreshChildren();
  }
  
  //
  // NodeSystem Overrides...
  //
  //  NodeSystem.AppendChild       - Append child node
  //  NodeSystem.PrependChild      - Prepend child node
  //  NodeSystem.InsertBeforeChild - Insert child node before another child
  //  NodeSystem.InsertAfterChild  - Insert child node after another child
  //

  @Override
  public Error AppendChild(Node child) {
	  if (child == null)
	    return new Error("Null child passed");
	  if ( child.getClass() != NodeSystemDirectory.class && child.getClass() != NodeSystemFile.class)
	    return new Error("Directories can only contain other directories or files");
	  return super.AppendChild(child);
  }
  
  @Override
  public Error PrependChild(Node child) {
	  if (child == null)
	    return new Error("Null child passed");
	  if (child.getClass() != NodeSystemDirectory.class && child.getClass() != NodeSystemFile.class)
	    return new Error("Directories can only contain other directories or files");
	  return super.AppendChild(child);
  }
 
  @Override
  public Error InsertBeforeChild(Node child,Node sibling) {
    if (child == null)
      return new Error("Null child passed");
    if (child.getClass() != NodeSystemDirectory.class && child.getClass() != NodeSystemFile.class)
      return new Error("Directories can only contain other directories or files");
    return super.AppendChild(child);
  }

  @Override
  public Error InsertAfterChild(Node child,Node sibling) {
    if (child == null)
      return new Error("Null child passed");
    if (child.getClass() != NodeSystemDirectory.class && child.getClass() != NodeSystemFile.class)
      return new Error("Directories can only contain other directories or files");
    return super.AppendChild(child);
  }

}
