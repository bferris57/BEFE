package bferris.core;

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

public class Error extends Object {

  //
  // Class Methods
  //

  //
  // Instance Members
  //

  String message;
  int    line;
  int    col;
  
  //
  // Instance Methods
  //

  public Error(String message) {
    if (message == null)
        this.message = "<Unspecified>";
    else
        this.message = "<Error> " + message;
    line = 0;
    col  = 0;
  }
  
  public Error Line(int line) {
  	this.line = line;
  	return this;
  }
  
  public Error Col(int col) {
  	this.col = col;
  	return this;
  }
  
  public Error LineCol(int line, int col) {
  	return Col(col).Line(line);
  }
  
  @Override
  public String toString() {
  	String s = "";
  	if (line > 0) {
  		s = "(Line "+line;
  		if (col > 0) {
  			s = s + "/"+col;
  		s = s + "): ";
  		}
  		s = s + message;
  	}
  	else
  		s = "Error: "+message;
  	return s;
  }
}
