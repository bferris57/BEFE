package bferris.values;

public class ValueNumber extends Value {

	// Instance members
	protected String number;
	
	//-------------------------------------------------------------------
	//
  // Method: ValueNumber - Constructors
	//
	//-------------------------------------------------------------------

	public ValueNumber() {
		number = null;
	}
	
	public ValueNumber(boolean b) {
	  if (b)
	  	number = "1";
	  else
	  	number = "0";
	}
	
	public ValueNumber(byte b) {
		byte[] barray = {b};
		number = new String(barray);
	}

	public ValueNumber(char c) {
		number = String.format("%d", (int)c);
	}
	
	public ValueNumber(short i) {
		number = String.format("%d",i);
	}

	public ValueNumber(int i) {
		number = String.format("%d",i);
	}

	public ValueNumber(long i) {
		number = String.format("%d",i);
	}

	public ValueNumber(float f) {
		number = String.format("%f",f);
		number = new NumberParser(number).ToString();
	}

	public ValueNumber(double f) {
		try {
  		number = String.format("%f",f);
		}
		catch (Exception e) {
			number = "NaN";
		}
		number = new NumberParser(number).ToString();
	}

	public ValueNumber(String value) {
		number = new NumberParser(value).ToString();
	}

	//-------------------------------------------------------------------
	//
	// Method: GetType() - Get a Value's Type
	//
	//-------------------------------------------------------------------

	public Value.Type GetType() {
		return Type.Number;
	}
	
	//-------------------------------------------------------------------
	//
	// Method: Set() - Set number value
	//
	//-------------------------------------------------------------------

	@Override
  public Value Set(boolean value)   {return new ValueNumber(value);}
	@Override
	public Value Set(byte value)      {return new ValueNumber(value);}
	@Override
	public Value Set(char value)      {return new ValueNumber(value);}
	@Override
	public Value Set(int value)       {return new ValueNumber(value);}
	@Override
	public Value Set(long value)      {return new ValueNumber(value);}
	@Override
	public Value Set(float value)     {return new ValueNumber(value);}
	@Override
	public Value Set(double value)    {return new ValueNumber(value);}
	@Override
	public Value Set(String value)    {return new ValueNumber(value);}

	@Override
	public Value Set(Value value) {

		Value that;
		
		if (value.GetType() == Type.Error ||
				value.GetType() == Type.String) {
			number = value.Get_String();
			if (!IsValid(number))
				number = "NaN";
			that = this;
		}
		else
			that = super.Set(value);
  
		return that;
	}

	//-------------------------------------------------------------------
	//
	// Method: Get_xxx() - Get() methods for different java types
	//
	//-------------------------------------------------------------------

	@Override
	public boolean   Get_boolean()     {
		boolean b;
		try {
			b = Boolean.parseBoolean(number);
		}
		catch (Exception e) {
			if (number == null)
				b = false;
			else
  			b = true;
	  }
		return (boolean)b;
	}
	
	@Override
	public byte      Get_byte() {
		byte b;
		try {
			b = Byte.parseByte(number);
		}
		catch (Exception e) {
			b = 0;
	  }
		return b;
	}

	
	@Override
	public char      Get_char() {
		int i = Get_int();
		if (i < 0 || i > 0x10ffff)
			i = 0;
		return (char)i;
	}

	@Override
	public int       Get_int() {
		int i;
		try {
		  i = Integer.parseInt(number);	
		}
		catch (Exception e) {
			i = 0;
		}
		return i;
	}
	
	@Override
	public long      Get_long() {
		long l;
		try {
		  l = Long.parseLong(number);	
		}
		catch (Exception e) {
			l = 0;
		}
		return l;
	}
	
	@Override
	public String    Get_String() {
		if (number == null)
			return "";
		return number;
	}
	
	@Override
	public Boolean   Get_Boolean() {
  	Boolean b;
  	try {
  		b = Boolean.parseBoolean(number);
  	}
  	catch (Exception e) {
  		if (number == null)
  			b = null;
  		else
  			b = true;
    }
  	return b;
	}
	
	@Override
	public Character Get_Character()   {
		return Get_char();
	}
	
	@Override
	public byte[]    Get_byteArray()   {return new byte[0];}
	
	@Override
	public char[]    Get_charArray()   {return number.toCharArray();}
	
	@Override
	public int[]     Get_intArray()    {return new int[0];}
	
	@Override
	public long[]    Get_longArray()   {return new long[0];}
	
	@Override
	public String[]  Get_StringArray() {String[] sarray = {number}; return sarray;}

	//-------------------------------------------------------------------
	//
	// Method: IsValid - Is this a valid string value for type?
	//
	//-------------------------------------------------------------------

  public boolean IsValid(Type type, String string) {

  	boolean result;
    String parse;
  	
  	processing: do {
  		
  	  result = true;	

  	  // Defer to ValueBase if not Number...
    	if (GetType() != Type.Number) {
        result = super.IsValid(type, string);
        break processing;
    	}
    	
      // Parse it
    	parse = new NumberParser().Parse(string);
    	result = (parse != null);

    	break processing;
      
  	} while (false);

  	return result;
  }

  public boolean IsValid(String s) {
  	return IsValid(Type.Number,s);
  }
  
	//-------------------------------------------------------------------
	//
	// Java standard methods
	//
	//-------------------------------------------------------------------
  
  @Override
  public String toString() {
  	if (number == null)
  		return "null";
  	return number;
  }

	//-------------------------------------------------------------------
	//
	// Private Classes
	//
	//-------------------------------------------------------------------

	static class NumberParser {

		String  source;
		String  parse;
		
		boolean isinf;
		boolean isnan;
		char    sign;
		String  intdigits;
		String  decdigits;
		char    expsign;
		String  expdigits;

		NumberParser() {
			Parse(null);
		}
		
		NumberParser(String string) {
			Parse(string);
		}
		
		boolean ValidParse() {
			if (source == null || parse == null)
				return false;
			return true;
		}
		
	  String Parse(String string) {
	  	
	  	String   result;
	  	char     c;
		  char     sign;
		  char     expsign;
	  	String[] parts;
	  	String   numpart;
	  	String   intpart;
	  	String   decpart;
	  	String   exppart;
	  	int      i;

	  	this.source    = string;
	  	this.parse     = null;
	  	
		  result    = null;
	  	sign      = 0;
  		expsign   = 0;
  		sign      = 0;
  		
  		// Initialise Parse class member
  		this.isinf     = false;
  		this.isnan     = true;
  		this.sign      = 0;
  		this.intdigits = "";
  		this.decdigits = "";
  		this.expsign   = 0;
  		this.expdigits = "";
  		
  		//
  		// Parse it...
  		//
  		
  		result = null;  		
  		processing: do {

  			if (string == null)
  				break processing;
	      string = string.trim().toLowerCase();
	      if (string.length() == 0)
	      	break processing;
	
	      // If "nan", we're finished
	      if (string.equals("nan")) {
	      	result = "NaN";
	      	break processing;
	      }
	      
	  		// Handle leading sign
	  		c = string.charAt(0);
	  		if (c == '+' || c == '-') {
	  			string = string.substring(1);
	  			sign = c;
	  		}
	  		if (string.length() == 0)
          break processing;
	  		
	  		// handle "inf" 
	  		if (string.equals("inf")) {
	  			result = "inf";
	  			if (sign == '-')
	  				result = sign+result;
	  			break processing;
	  		}
	  		
	  		// Pull apart exponent
	      parts = string.split("e");
	      if (parts.length == 0 || parts.length > 2)
	      	break processing;
	      if (string.length() > 0 && string.substring(string.length()-1).equals("e"))
	      	break processing;
	      
	      numpart = parts[0];
	      if (parts.length == 2)
	      	exppart = parts[1];
	     	else
	      	exppart = "";	
	      
	      // Strip leading sign off exponent part
	      if (exppart.length() > 0) {
	      	c = exppart.charAt(0);
	      	if (c == '+' || c == '-') {
	      		exppart = exppart.substring(1);
	      		expsign = c;
	      	}
	      	if (exppart.length() == 0)
  	      	break processing;
	      }
		  
	  	  // Pull apart number
	      parts = numpart.split("\\.");
	      if (parts.length == 0 || parts.length > 2)
	      	break processing;
	      intpart = parts[0];
	      if (parts.length == 2)
	      	decpart = parts[1];
	      else
	      	decpart = "";
	      
	      // Validate integer, decimal, and exponent digits
	      for (i=intpart.length()-1; i>=0; i--) {
	      	c = intpart.charAt(i);
	      	if (c < '0' || c > '9')
		      	break processing;
	      }
	      for (i=decpart.length()-1; i>=0; i--) {
	      	c = decpart.charAt(i);
	      	if (c < '0' || c > '9')
		      	break processing;
	      }
	      for (i=exppart.length()-1; i>=0; i--) {
	      	c = exppart.charAt(i);
	      	if (c < '0' || c > '9')
		      	break processing;
	      }
	      
	      // Strip leading digits off integer and exponent parts
	      // and trailing digits off decimal part
	      for (i=0;i<intpart.length()-1;) {
	      	c = intpart.charAt(i);
	      	if (c != '0') break;
	      	intpart = intpart.substring(1);
	      }
	      for (i=0;i<exppart.length()-1;) {
	      	c = exppart.charAt(i);
	      	if (c != '0') break;
	      	exppart = exppart.substring(1);
	      }
	      for (i=decpart.length()-1;i>=0;i--) {
	      	c = decpart.charAt(i);
	      	if (c != '0') break;
	      }
	      if (decpart.length() > 0)
	      	decpart = decpart.substring(0,i+1);
	      else
	      	decpart = "";
	
	      // Normalise zero
	      if (intpart.length() == 0 && decpart.length() == 0)
	      	intpart = "0";
	      
	      // Can't use sign if only zero
	      if (intpart.equals("0") && decpart.length() == 0 && sign != 0)
	        break processing;
	      
	      // Publish the parts
	      this.sign      = sign;
	      this.isnan     = false;
	      this.expsign   = expsign;
	      this.intdigits = intpart;
	      this.decdigits = decpart;
	      this.expdigits = exppart;
	      
        result = ToString();
        
  		} while (false);  // Finished all processing

  		this.parse = result;
  		
  		return result;
	  }
	  
	  String ToString() {
	  
	  	String result;

	  	if (isnan)
	  		return "NaN";
	  	if (isinf) {
	  		result = "inf";
	  		if (sign == '-')
	  			result = "-" + result;
	  		return result;
	  	}
	  	
      if (sign == '-')
      	result = "-";
      else
      	result = "";
      result = result + intdigits;
      if (decdigits.length() != 0)
      	result = result + "."+decdigits;
      if (expdigits.length() != 0) {
      	result = result + "e";
        if (expsign != '-')
        	result = result + "-";
        result = result + expdigits;
      }

	  	return result;
	  }
	  
	}
	
}
