package bferris.json;

public class JSONError extends JSON {

	public int    line;
	public int    col;
	public String message = "Unknown";

	public JSONError() {
		type = JSONType.Error;
    line = JSON.curline;
    col  = JSON.curcol;
    Finalise();
	}

	public JSONError(String message) {
		type = JSONType.Error;
    line = JSON.curline;
    col  = JSON.curcol;
	  this.message = message;
    Finalise();
	}
	  
	@Override
	public boolean IsEquivalentTo(JSON that) {
		return EquivalencyFailed();
	}
	
	@Override
	public JSON Finalise() {
		finalised = true;
		return this;
	}
	
	@Override
	public JSON GetValue() {
  	String s = "";
  	if (line > 0) {
  		s = "(Line "+line;
  		if (col > 0) {
  			s = s + "/" + col;
  		s = s + ")";
  		}
  		s = "JSON Error "+s+": "+message;
  	}
  	else
  		s = "JSON Error: "+message;
  	return new JSONString(s);
	}
	
	@Override
	public String ToString() {
		JSON jstr = GetValue();
		return jstr.ToString();
	}

  @Override
  public String toString() {
  	return GetValue().toString();
  }

}
