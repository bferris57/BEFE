package bferris.core;

import java.io.*;

import java.util.HashMap;

import bferris.node.*;
import bferris.json.*;

/**
 *
 * @author bferris
 */

public class Repository extends NodeBase {

  public  PrintStream            out = System.out;
  private HashMap<String,Object> objects;

  public Repository() {
  	super(Ids.TypeRepository);
    Init("<Unnamed Repository>");
  }

  public Repository(String pname) {
  	super(Ids.TypeRepository);
    Init(pname);
  }

  private void Init(String pname) {

    Object.SetRepository(this);
    SetName(pname);
    objects = new HashMap<String,Object>();
    
    return;
  }

  public Object GetObject(Id objid) {
    Object obj;
    if (objects.containsKey(objid))
    	obj = objects.get(objid);
    else
    	obj = null;
    return obj;
  }
  
  public Node GetNode(Id nodeid) {
  	Object node;
  	node = GetObject(nodeid);
  	if (node != null && !node.Is(NodeBase.class))
  		node = null;
  	return (Node)node;
  }
  
  @Override
  public JSON ToJSON(JSON out) {
  
  	JSONObject json;
  	JSONArray  typeids;
  	int        typeidx;
  	JSONObject idtypeids;
  	
  	out = super.ToJSON(out);
  	json = (JSONObject)out;

  	// Serialise typeids...
    typeids = new JSONArray();
    for (String objid: objects.keySet()) {
    	String typeid = objects.get(objid).GetTypeId().ToString();
    	for (typeidx=0;typeidx<typeids.GetSize();typeidx++) {
    		if (typeids.GetElement(typeidx).ToString().equals(typeid))
    			break;
    	}
    	if (typeidx>=typeids.GetSize())
      	typeids.AppendElement(new JSONString(typeid));
    }
    json.SetMember(new JSONMember("typeids",typeids));
    
    // Serialise idtypeids...
  	idtypeids = new JSONObject();
  	for (String id : objects.keySet()) {
  		Object obj;
  		obj = objects.get(id);
    	for (typeidx=0;typeidx<typeids.GetSize();typeidx++) {
    		if (typeids.GetElement(typeidx).ToString().equals(obj.GetTypeId().ToString()))
    			break;
    	}
    	idtypeids.SetMember(new JSONMember(id,new JSONNumber(typeidx)));
  		
  	}
   	json.SetMember(new JSONMember("idtypeids",idtypeids));
  	
  	return json;
  }
  
  @Override
  public Error SetParent(Node parent) {
  	if (parent != null)
    	return new Error("Repositories cannot have a parent");
  	return null;
  }
  
  public synchronized void Register(Object that) {
  
  	if (that != null) synchronized (that) {
  		if (!objects.containsKey(that.GetId().ToString())) {
  		  objects.put(that.GetId().ToString(),that);
  		}
  	}
  }
  
  public synchronized void Deregister(Object that) {
    
  	if (that != null) synchronized (that) {
  		if (objects.containsKey(that.GetId().ToString())) {
  		  objects.remove(that.GetId().ToString());
  		}
  	}
  }
  
  public Error SaveTo(String filename) {

  	Error       result;
  	File        file;
  	JSON        json;
    String[]    jsonlines;
    PrintStream ofile;
    
    // Form the file name
  	file = new File(filename);
    result = null;
    
  	while (true) {
  		
  		// Create the directories if not already there...
  		try {file.getParentFile().mkdirs();}
  		catch (Throwable e) {
  			result = new Error("Repository.SaveTo() failed to create parent directories for '"+filename+"'");
  			break;
  		}
  		
  	  // Open the file
  	  try {
    	  ofile = new PrintStream(new FileOutputStream(file));
  	  }
  	  catch (FileNotFoundException e) {
  	  	result = new Error("Repository.SaveTo() failed to create output file '"+filename+"'");
  	  	break;
  	  }
  	  
  	  // Get the JSON and write it
  	  json = ToJSON();
  	  if (json.IsError()) {
  	  	result = new Error("Repository.SaveTo() failed to create JSON: "+json.ToString());
  	  	break;
  	  }
  	  jsonlines = json.ToStringArray();
  	  for (String line : jsonlines) {
  	  	ofile.println(line);
  	  }
  	  ofile.close();
  	  
  	  break;
  	}
  	
  	return result;
  }
  
}
