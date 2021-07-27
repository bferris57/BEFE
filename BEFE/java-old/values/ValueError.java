package bferris.values;

public class ValueError extends Value {

	// Instance members
	String message;
	
	//-------------------------------------------------------------------
	//
  // Method: ValueError - Constructors
	//
	//-------------------------------------------------------------------

	protected ValueError() {
	}

	protected ValueError(String value) {
		message = value;
	}
	
	//-------------------------------------------------------------------
	//
	// Method: GetType() - Get a Value's Type
	//
	//-------------------------------------------------------------------

	public Value.Type GetType() {
		return Type.Error;
	}
	
	//-------------------------------------------------------------------
	//
	// Method: Set() - Set a value
	//
	//-------------------------------------------------------------------

	@Override
	public Value Set(String value) {

		if (value != null && message == null)
			message = value;
		return this;
	}
	
	@Override
	public Value Set(char[] value) {
		if (value == null)
			value = new char[0];
		return Set(new String(value));
	}
	
	public Value Set(Value value) {

		Value that;
		
		if (value.GetType() == Type.Error ||
				value.GetType() == Type.String) {
			message = value.Get_String();
			that = this;
		}
		else
			that = super.Set(value);
  
		return that;
	}

	//-------------------------------------------------------------------
	//
	// Method: Get_<javatype>() - Get value as Java base types
	//
	//-------------------------------------------------------------------

	@Override
	public String Get_String() {
		return "Error: "+message;
	}

	@Override
	public char[] Get_charArray() {
		return Get_String().toCharArray();
	}

	@Override
	public String[] Get_StringArray() {
		String[] sarray = new String[0];
		sarray[0] = Get_String();
		return sarray;
	}

	//-------------------------------------------------------------------
	//
	// Method: IsValid - Is this a valid string value for type?
	//
	//-------------------------------------------------------------------

  public boolean IsValid(Type type, String string) {

    return true;
  }

	//-------------------------------------------------------------------
	//
	// Java standard methods
	//
	//-------------------------------------------------------------------
  
  @Override
  public String toString() {
  	return message;
  }

}
