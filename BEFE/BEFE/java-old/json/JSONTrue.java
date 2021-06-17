package bferris.json;

public final class JSONTrue extends JSON {

	public JSONTrue() {
    type = JSONType.True;
    finalised = true;
	}
	
	@Override
	public JSON GetValue() {
		return new JSONString("true");
	}
	
	@Override
	public String ToString() {
	  return "true";	
	}

  @Override
  public String toString() {
  	return "true";
  }

}