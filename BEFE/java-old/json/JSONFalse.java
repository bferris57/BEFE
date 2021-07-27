package bferris.json;

public final class JSONFalse extends JSON {

	public JSONFalse() {
		type = JSONType.False;
    finalised = true;
	}

	@Override
	public JSON GetValue() {
		return new JSONString("false");
	}
	
	@Override
	public String ToString() {
	  return "false";	
	}
	
  @Override
  public String toString() {
  	return GetValue().toString();
  }
	
}