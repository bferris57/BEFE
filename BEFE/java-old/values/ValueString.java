package bferris.values;

public class ValueString extends Value {

	// Instance members
	String string;
	
	//-------------------------------------------------------------------
	//
  // Method: ValueError - Constructors
	//
	//-------------------------------------------------------------------

	protected ValueString() {
		string = "";
	}
	
	public ValueString(boolean b) {
		if (b)
			string = "true";
		else
			string = "false";
	}

	public ValueString(byte b) {
		byte[] barray = {b};
		string = new String(barray);
	}

	public ValueString(char c) {
		string = ""+c;
	}
	
	public ValueString(short i) {
		string = String.format("%d",i);
	}

	public ValueString(int i) {
		string = String.format("%d",i);
	}

	public ValueString(long i) {
		string = String.format("%d",i);
	}

	public ValueString(float f) {
		string = new ValueNumber(f).Get_String();
	}

	public ValueString(double d) {
		string = new ValueNumber(d).Get_String();
	}

	protected ValueString(String value) {
		string = value;
	}

	//-------------------------------------------------------------------
	//
	// Method: GetType() - Get a Value's Type
	//
	//-------------------------------------------------------------------

	public Value.Type GetType() {
		return Type.String;
	}
	
	//-------------------------------------------------------------------
	//
	// Method: Length() - Get the Value's length in entries
	//
	//-------------------------------------------------------------------

	@Override
	public Value Length() {
		if (string == null)
			return NullValue.Length();
		else
			return New(Type.Number).Set(string.length());
	}
	
	//-------------------------------------------------------------------
	//
	// Method: Set() - Set string value
	//
	//-------------------------------------------------------------------

	public Value Set(String value)    {string = value; return this;}

	public Value Set(Value value) {

		Value that;
		
		if (value.GetType() == Type.Error ||
				value.GetType() == Type.String) {
			string = value.Get_String();
			that = this;
		}
		else
			that = super.Set(value);
  
		return that;
	}

	public String    Get_String()      {return string;}
	public String[]  Get_StringArray() {String[] sarray = {string}; return sarray;}

	//-------------------------------------------------------------------
	//
	// Method: Get - Get identified object/array entry
	//
	//-------------------------------------------------------------------

	@Override
	public Value     Get(boolean identifier) {return NullValue;}
	@Override
  public Value     Get(byte    identifier) {return Get((int)identifier);}
	@Override
  public Value     Get(char    identifier) {return Get((int)identifier);}
	@Override
  public Value     Get(short   identifier) {return Get((int)identifier);}
	@Override
  public Value     Get(int     identifier) {return GetIndex(identifier);}
	@Override
  public Value     Get(long    identifier) {return Get((int)identifier);}
	@Override
  public Value     Get(float   identifier) {return Get((int)identifier);}
	@Override
  public Value     Get(double  identifier) {return Get((int)identifier);}  
	@Override
  public Value     Get(String  identifier) {return GetIndex(identifier);}
	@Override
	public Value     Get(Value   identifier) {return NullValue;}

	//-------------------------------------------------------------------
	//
	// Method: GetIndex - Return value of specified index in array
	//
	//-------------------------------------------------------------------

	public Value GetIndex(int index) {
		
		if (index < 0)
			index = string.length()+index;
		if (index < 0 || index >= string.length())
			return NullValue;
		return New(Type.Char).Set(string.charAt(index));
	}
	
	public Value GetIndex(String index) {
	
		ValueNumber number;
		number = (ValueNumber)Value.New(Type.Number);
		number = (ValueNumber)number.Set(index);
		if (!Value.IsNull(number)) {
			return GetIndex(number.Get_int());
		}
		return NullValue;
	}
	
	//-------------------------------------------------------------------
	//
	// Java standard methods
	//
	//-------------------------------------------------------------------
  
  @Override
  public String toString() {
  	return string;
  }

  @Override
  public int length() {
  	return string.length();
  }

  @Override
  public char charAt(int index) {
  	return Get(index).Get_char();
  }

  public char[] toCharArray() {
  	return string.toCharArray();
  }
  
}
