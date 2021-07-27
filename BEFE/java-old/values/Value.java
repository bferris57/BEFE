package bferris.values;

public class Value {

	protected static       boolean constraintypes;
	public    static final Value   NullValue = new ValueNull(); 
	
	public enum Type {
		// Primitive types...
		 Null             {Value New() {return Value.New(Null);    }}
		,Error            {Value New() {return Value.New(Error);   }}
		,Id               {Value New() {return Value.New(Id);      }}
		,Type             {Value New() {return Value.New(Type);    }}
		,Number           {Value New() {return Value.New(Number);  }}
		,Object           {Value New() {return Value.New(Object);  }}
		// Constrained level 1 types
		,Tuple            {Value New() {return Value.New(Tuple);   }}
		,Array            {Value New() {return Value.New(Array);   }}
		,String           {Value New() {return Value.New(String);  }}
		,Boolean          {Value New() {return Value.New(Boolean); }}
		,Bit              {Value New() {return Value.New(Bit);     }}
		,Byte             {Value New() {return Value.New(Byte);    }}
		,Char             {Value New() {return Value.New(Char);    }}
		,Short            {Value New() {return Value.New(Short);   }}
		,Int              {Value New() {return Value.New(Int);     }}
		,Long             {Value New() {return Value.New(Long);    }}
		,Float            {Value New() {return Value.New(Long);    }}
		,Double           {Value New() {return Value.New(Long);    }}
		,MAXTYPE          {Value New() {return Value.New(MAXTYPE); }};
		abstract Value New();
	}
	
	//-------------------------------------------------------------------
	//
  // Static methods
	//
	//-------------------------------------------------------------------

	public static final Value New(Type type) {

		Value result;
		
		switch (type) {		
		case Null:    result = new ValueNull();   break;
		case Error:   result = new ValueError();  break;
		case Id:      result = new ValueId();     break;
		case Type:    result = new ValueNull();   break;
		case String:  result = new ValueString(); break;		
		case Number:  result = new ValueNumber(); break;
		case Boolean: result = new ValueNumber(); break;
		case Bit:     result = new ValueNumber(); break;
		case Byte:    result = new ValueNumber(); break;
		case Char:    result = new ValueNumber(); break;
		case Short:   result = new ValueNumber(); break;
		case Int:     result = new ValueNumber(); break;
		case Long:    result = new ValueNumber(); break;
		case Float:   result = new ValueNumber(); break;
		case Double:  result = new ValueNumber(); break;
		default:  	  result = new ValueNull();    break;
		}
		
		return result;

	}

	public static final Value New(Type type, Value value) {

		return New(type).Set(value);

	}

	public static final Value New(Type type, String value) {

		return New(type).Set(value);

	}

  public static boolean IsError(Value that) {
  	if (that != null && that.GetType() == Type.Error)
  		return true;
  	return false;
  }
  
  public static boolean IsNull(Value that) {
  	if (that == null || that == NullValue || that.GetType() == Type.Null)
  		return true;
  	return false;
  }
  
	//-------------------------------------------------------------------
	//
	// Method: GetType() - Get a Value's Type
	//
	//-------------------------------------------------------------------

	public Value.Type GetType() {return NullValue.GetType();}
	
	//-------------------------------------------------------------------
	//
	// Method: Length() - Get a Value's length in entries
	//
	//-------------------------------------------------------------------

	public Value Length() {return NullValue;}

	//-------------------------------------------------------------------
	//
	// Method: Set() - Default Set() methods default behaviour
	//
	//-------------------------------------------------------------------

	public Value Set()                {return NullValue;}
	public Value Set(boolean value)   {return NullValue;}
	public Value Set(byte value)      {return NullValue;}
	public Value Set(char value)      {return NullValue;}
	public Value Set(short value)     {return NullValue;}
	public Value Set(int value)       {return NullValue;}
	public Value Set(long value)      {return NullValue;}
	public Value Set(float value)     {return NullValue;}
	public Value Set(double value)    {return NullValue;}
	public Value Set(String value)    {return new ValueString(value);}
	public Value Set(boolean[] value) {return NullValue;}
	public Value Set(byte[] value)    {return NullValue;}
	public Value Set(char[] value)    {return NullValue;}
	public Value Set(short[] value)   {return NullValue;}
	public Value Set(int[] value)     {return NullValue;}
	public Value Set(long[] value)    {return NullValue;}
	public Value Set(float[] value)   {return NullValue;}
	public Value Set(double[] value)  {return NullValue;}
	public Value Set(String[] value)  {return NullValue;}
	public Value Set(Value name,Value value)  {return NullValue;}

	public Value Set(Value value) {
		
		if (value == null)
			return NullValue;
		else if (constraintypes && value.GetType() != this.GetType())
			return New(Type.Error).Set("Type constrained to "+this.GetType());
		return value;
	}

	//-------------------------------------------------------------------
	//
	// Method: Get_<javatype>() - Get Java base types default behaviour
	//
	//-------------------------------------------------------------------

	public boolean   Get_boolean()     {return false;}
	public byte      Get_byte()        {return 0;}
	public char      Get_char()        {return 0;}
	public int       Get_int()         {return (int)0x80000000;}
	public long      Get_long()        {return (long)0x8000000000000000L;}
	public String    Get_String()      {return "";}
	public Boolean   Get_Boolean()     {return false;}
	public Character Get_Character()   {return 0;}
	public byte[]    Get_byteArray()   {return new byte[0];}
	public char[]    Get_charArray()   {return new char[0];}
	public int[]     Get_intArray()    {return new int[0];}
	public long[]    Get_longArray()   {return new long[0];}
	public String[]  Get_StringArray() {return new String[0];}

	//-------------------------------------------------------------------
	//
	// Method: Get - Get identified object/array entry
	//
	//-------------------------------------------------------------------

	public Value     Get(boolean identifier) {return NullValue;}
  public Value     Get(byte    identifier) {return NullValue;}
  public Value     Get(char    identifier) {return NullValue;}
  public Value     Get(short   identifier) {return NullValue;}
  public Value     Get(int     identifier) {return NullValue;}
  public Value     Get(long    identifier) {return NullValue;}
  public Value     Get(float   identifier) {return NullValue;}
  public Value     Get(double  identifier) {return NullValue;}  
  public Value     Get(String  identifier) {return NullValue;}
	public Value     Get(Value   identifier) {return NullValue;}
  
	//-------------------------------------------------------------------
	//
	// Method: IsValid - Is this a valid string value for type?
	//
	//-------------------------------------------------------------------

  public boolean IsValid(Type type, String string) {

  	Value v;
  	if (type == Type.Error && string != null)
  		return true;
  	v =	New(type).Set(string);
    if (v.GetType() == Type.Null || v.GetType() == Type.Error)
    	return false;
    return true;
  }
  
  //-------------------------------------------------------------------
	//
	// Java standard methods
	//
	//-------------------------------------------------------------------
  
  @Override
  public String toString() {
  	return "Unimplemented Value Type "+GetType().toString();
  }

  public char charAt(int index) {
  	return (char)0;
  }
  
  public int length() {
  	return 0;
  }
  
  public char[] toCharArray() {
  	return null;
  }
  
}
