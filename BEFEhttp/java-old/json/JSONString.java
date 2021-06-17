package bferris.json;

public final class JSONString extends JSON {

	private String  value;
	
	public JSONString() {
	  type = JSONType.String;
	}
	
	public JSONString(String s) {
		type = JSONType.String;
		value = s;
		Finalise();
	}
	
	@Override
	public boolean IsEquivalentTo(JSON that) {
		if (super.IsEquivalentTo(that)) {
			if (((JSONString)that).value.equals(value))
				return true;
		}
		return EquivalencyFailed();
	}
	
	@Override
	public JSON ProcessChar(char c) {
		
		JSON result;
	
		result = this;
		if (finalised)
			result = new JSONError("JSON string is finalised, cannot process character");
		else {

			// Start if of not already started
			if (value == null)
				value = "";
			
			// Handle characters...
			switch (c) {
  			case '\\':
  				if (value.length() > 0 && value.charAt(value.length()-1) != '\\')
    		  	value = value + c;
  		  	break;
			  case '"':
				  if (value.length() > 0 && value.charAt(value.length()-1) == '\\') {
				  	if (c != '\\')
  				  	value = value.substring(0,value.length()-1) + c;
				  	break;
				  }
			    value = value + c;
			    if (c == '"' && value.length() >= 2)
			    	result = this.Finalise();
 			    break;
 			  default:
    			if (c < 0x20)
	    			result = new JSONError(String.format("Invalid character 0x%02x in string, use '\\u00%02x' instead",(int)c,(int)c));
   			  else
   			  	value = value+c;
    			break;
  	  }
		}
		
		return result;
	
	}

	public JSON ProcessFrag(String frag) {
	
		if (frag == null)
			return null;
		if (frag.indexOf('"') >= 0)
			return new JSONError("JSONString.ProcessFrag() cannot process fragments containing '\"'");
		value = value + frag;
		return this;
	}
	
  @Override
  public JSON Finalise() {
  	
  	int     esc;
  	int     tesc;
  	char    esc1;
  	int     thechar;
  	int     utf16;
  	int     i;
  	
    if (finalised)
    	return new JSONError("JSON string already finalised");
    if (value == null || value.length() == 0)
    	return new JSONError("JSON string empty");
    if (value.charAt(0) != '"')
    	return new JSONError("JSON string does not start with a double quote ('\"') characters");
    if (value.charAt(value.length()-1) != '"')
    	return new JSONError("JSON string does not terminatd with a double quote ('\"') character");
    value = value.substring(1,value.length()-1);
    
    // Process all escape sequences...
    esc = 0;
    
    do {
    	tesc = value.substring(esc).indexOf('\\');
    	if (tesc < 0)
    		break;
    	esc += tesc;
    	
    	if (value.length()-esc <= 1)
    		return new JSONError("JSON string should end in double quote, not escaped quote ('\\\"')");
      esc1 = value.charAt(esc+1);
      if (esc1 == '\\' || esc1 == '"' || esc1 == '/') {
      	esc += 1;
      	continue;
      }
      else if (esc1 == 'b')	esc1 = '\b';
      else if (esc1 == 'f')	esc1 = '\f';
      else if (esc1 == 'n')	esc1 = '\n';
      else if (esc1 == 'r')	esc1 = '\r';
      else if (esc1 == 't')	esc1 = '\t';
      else if (esc1 == 'u') {
      	if (esc > value.length()-6)
        	return new JSONError("JSON unicode escape sequence truncated and invalid");
        utf16 = 0;
        for (i=0;i<4;i++) {
        	thechar = value.charAt(esc+2+i);
        	utf16 = utf16<<4;
        	if (thechar >= '0' && thechar <= '9')
        		thechar = thechar - '0';
        	else if (thechar >= 'a' && thechar <= 'f')
        		thechar = thechar - 'a' + 10;
       		else if (thechar >= 'A' && thechar <= 'F')
       			thechar = thechar - 'A' + 10;
       		else
       			return new JSONError("JSON unicode escape sequence ('\\uxxxx') contains non-hex digit '"+(char)thechar+"'");
        	utf16 = utf16 + thechar;
        }
        value = value.substring(0,esc) + (char)utf16 + value.substring(esc+6);
        esc += 1;
        continue;
      }
      else
      	return new JSONError("JSON String has invalid escape sequence '\\"+esc1+"' at offset "+(esc+1));
      value = value.substring(0,esc) + esc1 + value.substring(esc+2);
    } while (true);

    return super.Finalise();

  }
  
  @Override
  public JSON GetValue() {
  	return new JSONString(value);
  }
  
  @Override
  public String ToJSON() {
  	String sout;
  	int    i;
  	char   c;
  	
  	sout = "\"";
  	if (value != null) {
  		for (i=0;i<value.length();i++) {
  			c = value.charAt(i);
  			if (c == '"')
  				sout = sout + "\\\"";
  			//else if (c == '\'')
  			//	sout = sout + "\\'";
  			//else if (c == '/')
  			//	sout = sout + "\\/";
  			else if (c == '\b')
  				sout = sout + "\\b";
  			else if (c == '\f')
  				sout = sout + "\\f";
  			else if (c == '\n')
  				sout = sout + "\\n";
  			else if (c == '\r')
  				sout = sout + "\\r";
  			else if (c == '\t')
  				sout = sout + "\\t";
  			else if (c >= 0x20 && c <= 0x7f)
  				sout = sout + c;
  			else if (c < (char)0xffff) {
  				String HEXDIG = "0123456789ABCDEF";
  				int    j;
  			  j = (int)c;
  			  sout = sout + "\\u";
  			  sout = sout + HEXDIG.charAt((j>>12)& 0x0f);
  			  sout = sout + HEXDIG.charAt((j>>8) & 0x0f);
  			  sout = sout + HEXDIG.charAt((j>>4) & 0x0f);
  			  sout = sout + HEXDIG.charAt( j     & 0x0f);
  			}
  			else
  				throw new Error("Characters greater than 0xFFFF not currently handled");
  		}
  	}
  	sout = sout + "\"";
  	return sout;
  }

  @Override
  public String ToString() {
    if (value == null)
    	return "";
    else
    	return value;
  }

  @Override
  public String[] ToStringArray() {
  	String[] array = {""};
  	array[0] = ToJSON();
  	return array;
  }
  
  @Override
  public String toString() {
  	return ToString();
  }
}
