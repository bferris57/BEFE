package bferris.core;

import bferris.node.NodeBase;

public class Ids {

  public  static Id      TypeObject              = new Id("{80000000-ffff-ffff-ffff-ffffffffffff}");
  public  static Id      TypeRepository          = new Id("{80000001-ffff-ffff-ffff-ffffffffffff}");
  public  static Id      TypeNode                = new Id("{80010000-ffff-ffff-ffff-ffffffffffff}");
  public  static Id      TypeSystem              = new Id("{80020000-ffff-ffff-ffff-ffffffffffff}");
  public  static Id      TypeDisk                = new Id("{80020001-ffff-ffff-ffff-ffffffffffff}");
  public  static Id      TypeDirectory           = new Id("{80020002-ffff-ffff-ffff-ffffffffffff}");
  public  static Id      TypeFile                = new Id("{80020003-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSON                = new Id("{80030000-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONError           = new Id("{80030001-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONNull            = new Id("{80030002-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONTrue            = new Id("{80030003-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONFalse           = new Id("{80030004-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONObject          = new Id("{80030005-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONArray           = new Id("{80030006-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONNumber          = new Id("{80030007-ffff-ffff-ffff-ffffffffffff}");
  //public  static Id      TypeJSONString          = new Id("{80030008-ffff-ffff-ffff-ffffffffffff}");

  public final static Id[] FixedIds = {TypeObject
  	                                  ,TypeRepository
  	                                  ,TypeNode
  	                                  ,TypeSystem
  	                                  ,TypeDisk
  	                                  ,TypeDirectory
  	                                  ,TypeFile
  };

  // Factory...
  private static Object factory;
  static {

  	if (factory == null) {
  		factory = new Object();
  	  new Repository();
  	  new NodeBase();
  	}
  }
  
  public static Object New(Id typeid) {
  	return factory.New(typeid);
  }
}
