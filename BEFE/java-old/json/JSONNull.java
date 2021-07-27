package bferris.json;

public final class JSONNull extends JSON {

  public JSONNull() {
    type = JSONType.Null;
    finalised = true;
  }

  @Override
  public JSON GetValue() {
  	return NullObject;
  }
  
	@Override
	public String ToString() {
	  return "null";	
	}
	
	@Override
	public String toString() {
	  return "null";
	}

}
