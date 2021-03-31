//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// File:      NodeSystemFile.java - <Description>
//
// Author:    bferris
//
// Copyright: 2011, Bruce Ferris
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//
// Imports
//

package bferris.node;

import javax.activation.*;
import bferris.core.Error;
import bferris.core.Id;
import bferris.core.Ids;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            NodeSystemFile - A raw system file (e.g. binary bytes)
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

public class NodeSystemFile extends NodeSystemDirectory {

  //
  // Class Members
  //

	private final MimetypesFileTypeMap mimemap = new javax.activation.MimetypesFileTypeMap();
	
  //
  // Class Methods
  //

  //
  // Instance Members
  //

  //
  // Instance Methods
  //

  public NodeSystemFile(String name) {
    super(Ids.TypeFile);
    this.name = name;
    Init();
  }

  public NodeSystemFile(Id typeid) {
  	super(typeid);
  	Init();
  }
  
  private void Init() {
    src      = "";
    fullpath = "";
    //RefreshChildren();
  }

  public String GetMimeType() {
    return mimemap.getContentType(GetName());  	
  }
  
  //
  // Node Overrides...
  //
  //  Node.SetName           - Set Node name
  //  Node.AppendChild       - Append child node
  //  Node.PrependChild      - Prepend child node
  //  Node.InsertBeforeChild - Insert child node before another child
  //  Node.InsertAfterChild  - Insert child node after another child
  //  Node.OpenChildren
  //  Node.CloseChildren
  //
  
  @Override
  public Error AppendChild(Node child) {
  	return new Error("Cannot add child nodes to a System File");
  }
  
  @Override
  public Error PrependChild(Node child) {
	  return new Error("Cannot add child nodes to a System File");
  }
 
  @Override
  public Error InsertBeforeChild(Node child,Node sibling) {
	  return new Error("Cannot add child nodes to a System File");
  }

  @Override
  public Error InsertAfterChild(Node child,Node sibling) {
	  return new Error("Cannot add child nodes to a System File");
  }
  
}

