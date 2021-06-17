//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// File: Object.java - All objects are of this class
//
// Author:    bferris
//
// Copyright: 2011, Bruce Ferris
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

package bferris.core;

//
// Imports
//

import java.util.HashMap;

import bferris.json.*;
import bferris.graph.Vertex;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            Error - <Description>
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

public class Object extends Vertex implements Factory {
	
  //
  // Private class members
  //
	
  private   static HashMap<String,Factory> TypeIdToFactory;
  protected static Repository              repos;
  
  //
  // Public class methods
  //

  public static Factory GetFactory(Id typeid) {
  	String typeidstr;
    typeidstr = typeid.toString();
    if (TypeIdToFactory != null && TypeIdToFactory.containsKey(typeidstr))
    	return TypeIdToFactory.get(typeidstr);
    return null;
  }
  
  public static Repository SetRepository(Repository r) {
    Repository oldrepos;

    oldrepos = repos;
    repos = r;
    return oldrepos;
  }

  public static Repository GetObjectRepository() {
    return repos;
  }

  private static Object NullObject;
  
  public static Object NullObject() {
  	if (NullObject == null)
  		NullObject = new Object();
  	return NullObject;
  }

  public static boolean IsError(Object that) {
  	if (that == null || !that.Is(Error.class))
  		return false;
  	return true;
  }
  
  //
  // Private instance members
  //

  private Id         typeid;
  
  //
  // Public instance methods
  //
  
  public Object New() {
 		return new Object();
  }
  
  public Object New(Id typeid) {
  	
  	String  typeidstr;
  	Factory factory;
  	Object  that;
  	
  	if (TypeIdToFactory == null || typeid == Ids.TypeObject)
  		that = New();
  	else {
    	typeidstr = typeid.toString();
    	if (TypeIdToFactory.containsKey(typeidstr)) {
    		factory = TypeIdToFactory.get(typeidstr);
    		if (factory != this)
    			that = factory.New(typeid);
    		else
    			that = this;
    	}
    	else
    		that = null;
  	}

  	return that;
  	
  }

  public void RegisterFactory() {
  	String typeidstr = GetTypeId().ToString();
  	typeidstr = typeid.ToString();
    if (TypeIdToFactory == null)
    	TypeIdToFactory = new HashMap<String,Factory>();
    if (!TypeIdToFactory.containsKey(typeidstr))
    	TypeIdToFactory.put(typeidstr,this);  	
  }
  
  public boolean Is(Class<?> c) {
	  return c.isInstance(this);
  }
  
  public boolean Is(Id typeorid) {
  	return (id.equals(typeorid) || typeid.equals(typeorid));
  }

  public Object() {
    Init(null,Ids.TypeObject);
  }

  public Object(Id typeid) {
    Init(null,typeid);
  }

  public Object(Id id, Id typeid) {
    Init(id, typeid);
  }

  private void Init(Id id, Id typeid) {

    if (id == null)
    	id = new Id();
    if (typeid == null)
    	typeid = Ids.TypeObject;
    this.id     = id;
    this.typeid = typeid;

    RegisterFactory();

    //if (repos == null)
    //  repos = new Repository("Object.Object()");
    
    this.id     = id;
    this.typeid = typeid;

    // Register with Repository if we have one
  	if (repos != null)
  		repos.Register(this);
  	
  }

  public Id GetId() {
    return id;
  }

  public Id GetTypeId() {
  	if (typeid == null)
  		typeid = Ids.TypeObject;
    return typeid;
  }

  public boolean IsPersistent() {
  	return false;
  }
    

  public JSON ToJSON() {
  	return ToJSON(null);
  }
  
  public JSON ToJSON(JSON out) {
    
  	JSONObject jsonobj;
    
  	if (out == null || !out.IsObject())
  		out = JSONObject.NewOrdered();
  	
  	jsonobj = (JSONObject)out;
   	jsonobj.SetMember(new JSONMember("id",new JSONString(GetId().ToString())));
 	  jsonobj.SetMember(new JSONMember("type",new JSONString(GetTypeId().ToString())));

    return jsonobj;	
  }
  
  @Override
  public String toString() {
  	if (this == NullObject())
    	return "null";
  	else
  		return super.toString();
  }
}
